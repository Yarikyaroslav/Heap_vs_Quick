#include <iostream>
#include <random>
#include <vector>
#include <chrono>

using namespace std;

void qsort (vector<int>& arr, int left, int right){
    int i = left, j = right;
    int pivot = arr[ (left+right)/2 ];

    while (i <= j)
    {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }

            i++; j--;
        }

    }
    if (left < j) qsort (arr, left, j);
    if (i < right) qsort (arr, i, right);
}

void sift(vector<int>& arr,int left,int right){
    int i = left;
    int j = 2*left;
    int x = arr[left];
    if((j<right)&&(arr[j+1]>arr[j])) j++;
    while ((j<=right)&&(arr[j]>x)){
        arr[i] = arr[j]; i = j; j*=2;
        if((j<right)&&(arr[j+1]>arr[j])) j++;
    }
    arr[i] = x;
}

void hsort(vector<int>& arr){
    int N = arr.size();
    for (int L = int(N/2);L>-1;L--){
        sift(arr,L,N-1);
    }
    int R = N - 1;
    while (R>0){
        int x = arr[0];arr[0] = arr[R];arr[R] = x;
        R--; sift(arr,0,R);
    }
}

int main() {
    int N = 10;
    random_device rd;
    mt19937 mt(rd());
    vector < vector <int> > V(2*N+1, vector <int> (N) );
    for(int i=0;i<N;i++){ V[N][i] = mt()%(N);}
    for(int j=N+1;j<2*N+1;j++){
        V[j][0] = V[j-1][0];
        for(int i=1;i<N;i++){
            if(V[j-1][i]>V[j][i-1]){
                V[j][i] = V[j-1][i];
            }
            else{
                V[j][i] = V[j][i-1];
                V[j][i-1] = V[j-1][i];
            }
        }
    }

    for(int j=N-1;j>-1;j--){
        V[j][N-1] = V[j+1][N-1];
        for(int i=N-2;i>-1;i--){
            if(V[j+1][i]>V[j][i+1]){
                V[j][i] = V[j+1][i];
            }
            else{
                V[j][i] = V[j][i+1];
                V[j][i+1] = V[j+1][i];
            }
        }
    }


    for(int k=0;k<2*N+1;k+=100){
        auto start1 = chrono::steady_clock::now();
        for(int times = 0;times<10000;times++){auto tmp = V[k]; qsort(tmp,0,N-1);}
        auto end1 = chrono::steady_clock::now();
        chrono::duration<double> diff1 = end1 - start1;
        double time1 = diff1.count();

        auto start2 = chrono::steady_clock::now();
        for(int times = 0;times<10000;times++){auto tmp = V[k]; hsort(tmp);}
        auto end2 = chrono::steady_clock::now();
        chrono::duration<double> diff2 = end2 - start2;
        double time2 = diff2.count();

        cout<<double(k)/(2*N)*100<<'\t'<<time1*100<<'\t'<<time2*100<<'\n';
    }

    cout<<'\n';

    for(int size = 1e7;size<=2e7;size+=1e6){
        vector<int> tmp1(size);
        for(int i=0;i<size;i++){ tmp1[i] = mt()%(size);}
        auto tmp2 = tmp1;


        auto start1 = chrono::steady_clock::now();
        qsort(tmp1,0,size-1);
        auto end1 = chrono::steady_clock::now();
        chrono::duration<double> diff1 = end1 - start1;
        double time1 = diff1.count();

        auto start2 = chrono::steady_clock::now();
        hsort(tmp2);
        auto end2 = chrono::steady_clock::now();
        chrono::duration<double> diff2 = end2 - start2;
        double time2 = diff2.count();

        cout<<size<<'\t'<<time1<<'\t'<<time2<<'\n';
    }

}
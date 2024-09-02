#include "bigint.h"
#include <time.h>
#include <chrono>

using namespace std::chrono;
int main()
{
    BigInt num;
    BigInt i(3);
    long long int q = 0;
    std::cin>>num;
    BigInt sgrt;
    sgrt = sqrt(num);
    clock_t start = clock();
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();

    while(i <= sgrt){
        if(num % 2 == 0){
            num = num / 2;
            q++;
            continue;
        }
        while(num % i == 0){
            std::cout<<i<<" ";
            num = num / i;
        }
        i = i + 2;
    }
    if(i > sgrt) std::cout<<num<<" ";
    std::cout<<"2^"<<q;

    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //duration<double, std::milli> time_span = t2 - t1;
    //std::cout<<std::endl<<time_span.count()<< " milliseconds";

    clock_t end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<t<<" seconds";
    return 0;
}

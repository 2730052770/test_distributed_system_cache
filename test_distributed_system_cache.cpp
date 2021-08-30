#include<bits/stdc++.h>
using namespace std;
const int NUMSERVERS = 64;
const int NUMQUERIES = 1 << 20;
const int CACHESIZE = NUMSERVERS * log(NUMSERVERS) * 8;
const int ITERTIMES = 10;
unsigned int randu(){
	return rand() << 16 ^ rand();
}
void test(int ns, int nk, int nq, int cs){
	int * hash = new int[nk];
	int * cntkey = new int[nk];
	int * cntserver = new int[ns];
	double avgmaxratio = 0;
	
	for(int t = 0; t < ITERTIMES; t++) // 10 times, and calculate average
	{
		// random hash-table, key to server
		for(int i = 0; i < nk; i++) hash[i] = randu() % ns; 
		
		// create keys' retrieved times, retrieved times are equal
		int basenum = nq / nk;
		int numaddone = nq % nk;
		for(int i = 0; i < numaddone; i++) cntkey[i] = basenum + 1;
		for(int i = numaddone; i < nk; i++) cntkey[i] = basenum; // select a key
		for(int i = 0; i < cs && i < nk; i++) cntkey[i] = 0; // cached
		
		// calculate server load
		memset(cntserver, 0, sizeof(int) * ns);
		for(int i = 0; i < nk; i++) cntserver[hash[i]] += cntkey[i];
		
		// calculate max{server-thoughput/total-throughput} 
		double maxratio = 0;
		for(int i = 0; i < ns; i++)
			maxratio = max(maxratio, 1.0*cntserver[i]*ns/nq);
		avgmaxratio += maxratio;
	}
	avgmaxratio /= ITERTIMES;
		
	delete [] cntserver;
	delete [] cntkey;
	delete [] hash;

	printf("NSERVERS: %d, NKEYS: %d, NQUERIES: %d, CACHESIZE: %d, AVGMAXRATIO: %.3lf\n", ns, nk, nq, cs, avgmaxratio);
}
int main()
{
	for(int numkeys = 4; numkeys < NUMQUERIES; numkeys *= 2){
		int num[4] = {numkeys, numkeys / 4 * 5, numkeys / 2 * 3, numkeys / 4 * 7};
		for(int i = 0; i < 4; i++){
			test(NUMSERVERS, num[i], NUMQUERIES, 0);
			test(NUMSERVERS, num[i], NUMQUERIES, CACHESIZE); // At 3W ~ 8W, it has max ratio
		}
	}
    return 0;
}

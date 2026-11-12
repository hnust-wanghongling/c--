#include <bits/stdc++.h>
using namespace std;
#define ll long long

void solve(){
	ll n,m,x,sum;
	cin>>n;
	vector<ll> v(n);
	for(int i=0;i<n;i++){
		
			cin>>v[i];
			sum+=v[i];
		}
		sort(v.begin(),v.end());

		cout<<sum-(v[n-1]-v[n-2])<<endl;
	}	


int main(){
	ios::sync_with_stdio(0);
	cin.tie(0);cout.tie(0);
	solve();
	return 0;
}
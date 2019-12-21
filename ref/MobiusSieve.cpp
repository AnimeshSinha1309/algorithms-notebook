class Multiplicative {
// This is the definition for PHI
#define fn_prime_values(prime) (prime - 1)
#define fn_non_coprime(num, prime) (fn[num] * prime)
   public:
    ll size;
    vector<ll> fn;
    vector<ll> primes;
    vector<ll> lowest_prime_factor;
    Multiplicative(ll size) {
        size = size;
        lowest_prime_factor = vector<ll>(size, 0);
        fn = vector<ll>(size, 0);
        // https://stackoverflow.com/questions/34260399
        // linear sieve
        for (ll i = 2; i < size; i++)
            lowest_prime_factor[i] = i;
        // put any specific initialization code here like
        // multiplicativeFn[1] = 1;
        for (ll i = 2; i < size; i++) {
            if (lowest_prime_factor[i] == i) {
                fn[i] = fn_prime_values(i);
                primes.push_back(i);
            }
            for (auto p : primes) {
                ll ith_multiple = i * p;
                if (ith_multiple >= size)
                    break;
                lowest_prime_factor[ith_multiple] =
                    min(lowest_prime_factor[i], p);

                if (i % p) {
                    fn[ith_multiple] = fn[i] * fn[p];
                } else {
                    fn[ith_multiple] = fn_non_coprime(i, p);
                    break;
                }
            }
        }
    }
};

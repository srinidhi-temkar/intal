#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intal.h"

#define MAX 1001

// Library "intal" - Integer of arbitray length
// intal is a nonnegative integer of arbitrary length (not more than 1000 decimal digits).
// The integer is stored as a null terminated string of ASCII characters.
// String of decimal digits ('0' thru '9') are stored in big endian style.
// That is, the most significant digit is at the head of the string.
// Eg: Integer 25 is stored in str as '2' at str[0], '5' at str[1], and null char at str[2].

static char* set_intal_zero() {
	char* result = (char*)malloc(2*sizeof(char));
	strcpy(result, "0");
	return result;
}

static char* set_intal_one() {
	char* result = (char*)malloc(2*sizeof(char));
	strcpy(result, "1");
	return result;
}

static char* intal_copy(const char *intal) {
	int result_size = strlen(intal)+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	strcpy(result, intal);
	return result;
}

static int get_value(const char *intal, int len, int index) {
	if (index < len)
        return intal[len - index - 1] - '0'; 
    return 0;
}

static char* strip_leading_zeroes(char *nintal, int size) {
	if(size <= 1) {
		if(size == 1) nintal[0] = '\0';
		return nintal;
	}
	char *result;
	int f = 0;
	while(f < size && nintal[f++] == '0');
	if(f == size) {
		free(nintal);
		result = set_intal_zero();
		return result;
	}
	else {
		if(f == 0) {
			return nintal;
		}
		f--;
		result = (char*)malloc((size-f)*sizeof(char));
		memcpy(result, nintal+f, size-f);
		free(nintal);
		return result;
	}
}

static char* intal_by_two(const char *intal) {
	int len = strlen(intal);
	int result_size = len+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	result[result_size-1] = '\0';
	int d, quotient, remainder=0;
	for(int i=len-1; i>=0; i--) {
		d = get_value(intal, len, i);
		d = remainder*10 + d;
		quotient = d/2;
		if(d%2 == 1) {
			remainder = 1;
		} else {
			remainder = 0;
		}
		result[result_size-i-2] = quotient + '0';
	}
	return strip_leading_zeroes(result, result_size);
}

static void merge(char **arr, int l, int m, int r) {
	int i, j, k;
	int l1 = m-l+1;
	int l2 = r-m;

	char **arr_l = (char**)malloc(l1 * sizeof(char*));
	char **arr_r = (char**)malloc(l2 * sizeof(char*));

	for(i=0; i<l1; i++) {
		arr_l[i] = arr[l+i];
	}
	for(i=0; i<l2; i++) {
		arr_r[i] = arr[m+1+i];
	}
	i=0; j=0; k=l;
	while(i<l1 && j<l2) {
		if(intal_compare(arr_l[i], arr_r[j]) <= 0) {
			arr[k] = arr_l[i];
			i++;
		}
		else {
			arr[k] = arr_r[j];
			j++;
		}
		k++;
	}
	while(i < l1) {
		arr[k] = arr_l[i];
		i++;
		k++;
	}
	while(j < l2) {
		arr[k] = arr_r[j];
		j++;
		k++;
	}
	free(arr_l);
	free(arr_r);
}

static void merge_sort(char **arr, int l, int r) {
	if(l < r) {
		int m = l+(r-l)/2; 
		merge_sort(arr, l, m);
		merge_sort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}

// Returns the sum of two intals.
char* intal_add(const char* intal1, const char* intal2) {
	int l1, l2, width, i, sum, carry=0, d1, d2;
	l1 = strlen(intal1);
	l2 = strlen(intal2);
	width = l1 > l2 ? l1 : l2;
	char *result = (char *)malloc((width+2)*sizeof(char));
	result[width+1] = '\0';
	for(i=0; i<width; i++) {
		d1 = get_value(intal1, l1, i);
		d2 = get_value(intal2, l2, i);
		sum = d1 + d2 + carry;
		carry = sum / 10;
		sum = sum % 10;
		result[width-i] = sum+'0';
	}
	result[0] = carry+'0';
	return strip_leading_zeroes(result, width+2);
}

// Returns the comparison value of two intals.
// Returns 0 when both are equal.
// Returns +1 when intal1 is greater, and -1 when intal2 is greater.
int intal_compare(const char* intal1, const char* intal2) {
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);
	if(l1 > l2) {
		return 1;
	}
	else if(l2 > l1) {
		return -1;
	}
	else {
		for(int i=0; i<l1; i++) {
			if(intal1[i] > intal2[i]) {
				return 1;
			}
			else if(intal1[i] < intal2[i]) {
				return -1;
			}
		}
		return 0;
	}
}

// Returns the difference (obviously, nonnegative) of two intals.
char* intal_diff(const char* intal1, const char* intal2) {
	if(intal_compare(intal1, intal2) < 0) {
		const char *temp = intal2;
		intal2 = intal1;
		intal1 = temp;
	}
	int l1, l2, width, i, diff, borrow=0, d1, d2;
	l1 = strlen(intal1);
	l2 = strlen(intal2);
	width = l1 > l2 ? l1 : l2;
	char *result = (char *)malloc((width+1)*sizeof(char));
	result[width] = '\0';
	for(i=0; i<width; i++) {
		d1 = get_value(intal1, l1, i);
		d2 = get_value(intal2, l2, i);
		diff = d1 - d2 - borrow;
		borrow = 0;
		if(diff < 0) {
			borrow = 1;
			diff += 10;
		}
		result[width-i-1] = diff+'0';
	}
	return strip_leading_zeroes(result, width+1);
}

// Returns the product of two intals.
char* intal_multiply(const char* intal1, const char* intal2) {
	int l1, l2, i, j, d1, d2, prod, carry=0;
	l1 = strlen(intal1);
	l2 = strlen(intal2);
	int result_size = l1+l2+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	result[result_size-1] = '\0';
	memset(result, '0', result_size-1);
	for(i=0; i<l1; i++) {
		d1 = get_value(intal1, l1, i);
		if(d1 != 0) { 
			for(j=0; j<l2; j++) {
				d2 = get_value(intal2, l2, j);
				prod = d1 * d2 + carry + result[result_size-i-j-2] - '0';
				carry = prod / 10;
				prod = prod % 10;
				result[result_size-i-j-2] = prod + '0';
			}
			if(carry) {
				result[result_size-i-j-2] = carry + '0';
			}
		}
		carry = 0;
	}
	return strip_leading_zeroes(result, result_size);
}

// Returns intal1 mod intal2
// The mod value should be in the range [0, intal2 - 1].
// intal2 > 1
// Implement a O(log intal1) time taking algorithm.
// O(intal1 / intal2) time taking algorithm may exceed time limit.
// O(intal1 / intal2) algorithm may repeatedly subtract intal2 from intal1.
// That will take intal1/intal2 iterations.
// You need to design a O(log intal1) time taking algorithm.
// Generate your own testcases at https://www.omnicalculator.com/math/modulo
char* intal_mod_bs(const char* intal1, const char* intal2) {
	const char *dividend = intal1, *divisor = intal2;
	int cmp;
	char *result;
	cmp = intal_compare(dividend, divisor);
	if(cmp == 0) {
		result = set_intal_zero();
		return result;
	}
	if(cmp == -1) {
		result = intal_copy(dividend);
		return result;
	}
	char *low, *high;	
	low = set_intal_one();
	high = intal_copy(dividend);
	// Trying to find the quotient(mid) that will give the remainder(mod) which is >=0 and <divisor
	while(intal_compare(low, high) <= 0) {
		char *temp = intal_add(low, high);
		char *mid = intal_by_two(temp);
		free(temp);
		char *mids_dividend = intal_multiply(divisor, mid);
		if(intal_compare(mids_dividend, dividend) == 1) { // remainder < 0
			char *prev_high = high;
			high = intal_diff(mid, "1");
			free(prev_high);
			free(mid);
			free(mids_dividend);
			continue;
		}
		char *mids_remainder = intal_diff(dividend, mids_dividend);
		cmp = intal_compare(mids_remainder, divisor);
		if(cmp == 1) { // remainder > divisor
			char *prev_low = low;
			low = intal_add(mid, "1");
			free(prev_low);
		}
		else { // 0 <= remainder <= divisor
			if(cmp == 0) { // remainder == divisor
				result = set_intal_zero();
			}
			result = mids_remainder;
			free(mid);
			free(mids_dividend);
			free(low);
			free(high);
			return result;
		}
		free(mid);
		free(mids_dividend);
		free(mids_remainder);
	}
	free(low);
	return high;
}
char* intal_mod(const char* intal1, const char* intal2) {
	char *dividend = intal_copy(intal1);
	if(intal_compare(dividend, intal2) == -1) {
		return dividend;
	}
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);
	char *modded_divisor = (char*)calloc((l1+1), sizeof(char));
	strcpy(modded_divisor, intal2);
	memset(modded_divisor+l2, '0', l1-l2);
	int i = l1;
	while(i >= l2) {
		while(intal_compare(dividend, modded_divisor) >= 0) {
			char *temp = dividend;
			dividend = intal_diff(dividend, modded_divisor);
			free(temp);
		}
		modded_divisor[--i] = '\0';
	}
	free(modded_divisor);
	return dividend;
}

// Returns intal1 ^ intal2.
// Let 0 ^ n = 0, where n is an intal.
// Implement a O(log n) intal multiplications algorithm.
// 2^3000 has less than 1000 decimal digits. 3000 intal multiplications may exceed time limit.
char* intal_pow(const char* intal1, unsigned int n) {
	char *result;
	if(intal1[0] == '0') {
		result = set_intal_zero();
		return result;
	}
	result = set_intal_one();
	if(n == 0 || (intal1[0] == '1' && intal1[1] == '\0')) {
		return result;
	}
	char *temp_intal = intal_copy(intal1);
	while(n > 0) {
		if(n & 1) {
			char *temp1 = result;
			result = intal_multiply(result, temp_intal);
			free(temp1);
		}
		n = n >> 1;
		char *temp2 = temp_intal;
		temp_intal = intal_multiply(temp_intal, temp_intal);
		free(temp2);
	}
	free(temp_intal);
	return result;
}

// Returns Greatest Common Devisor of intal1 and intal2.
// Let GCD be "0" if both intal1 and intal2 are "0" even though it is undefined, mathematically.
// Use Euclid's theorem to not exceed the time limit.
char* intal_gcd(const char* intal1, const char* intal2) {
	char *a, *b;
	if(intal1[0] == '0' && intal2[0] == '0') {
		a = set_intal_zero();
		return a;
	}
	a = intal_copy(intal1);
	b = intal_copy(intal2);
	while(intal_compare(b, "0") != 0) {
		char *r = intal_mod(a, b);
		char *temp = a;
		a = b;
		free(temp);
		b = r;
	}
	free(b);
	return a;
}

// Returns nth fibonacci number.
// intal_fibonacci(0) = intal "0".
// intal_fibonacci(1) = intal "1".
char* intal_fibonacci(unsigned int n) {
	char *a, *b, *c;
	a = set_intal_zero();
	if(n == 0) {
		return a;
	}
	b = set_intal_one();
	for(int i=2; i <= n; i++) {
		c = intal_add(a, b);
		char* temp = a;
		a = b;
		free(temp);
		b = c;
	}
	free(a);
	return b;
}

// Returns the factorial of n.
char* intal_factorial(unsigned int n) {
	char *result;
	if(n >= 10000) {
		result = set_intal_zero();
		return result;
	}
	result = set_intal_one();
	char *intal_i = (char*)malloc((4+1) * sizeof(char));
	for(unsigned int i=2; i<=n; i++) {
		sprintf(intal_i, "%u", i);
		char *temp = result;
		result = intal_multiply(result, intal_i);
		free(temp);
	}
	free(intal_i);
	return result;
}

// Returns the Binomial Coefficient C(n,k).
// 0 <= k <= n
// C(n,k) < 10^1000 because the returning value is expected to be less than 10^1000.
// Use the Pascal's identity C(n,k) = C(n-1,k) + C(n-1,k-1)
// Make sure the intermediate intal values do not cross C(n,k).
// Use Dynamic Programming. Use extra space of not more than O(k) number of intals. Do not allocate the whole O(nk) DP table.
// Don't let C(1000,900) take more time than C(1000,500). Time limit may exceed otherwise.
char* intal_bincoeff(unsigned int n, unsigned int k) {
	if(k>n) {
		char* result = set_intal_zero();
		return result;
	}
	if(k>n/2) {
        k=n-k;
    }
	char **table = (char**)malloc((k+1) * sizeof(char*));
	table[0] = set_intal_one();
	for(int i=1; i < k+1; i++) {
		table[i] = set_intal_zero();
	}
	for(int i=1; i <= n; i++) {
		for(int j = (i<k ? i : k); j > 0; j--) {
			char *temp = table[j];
			table[j] = intal_add(table[j], table[j-1]);
			free(temp);
		}
	}
	char *result = intal_copy(table[k]);
	for(int i=0; i < k+1; i++) {
		free(table[i]);
	}
	free(table);
	return result;
}

// Returns the offset of the largest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_max(char **arr, int n) {
	int max_index = 0, i = 1;
	while(i < n) {
		if(intal_compare(arr[i], arr[max_index]) == 1) {
			max_index = i;
		}
		i++;
	}
	return max_index;
}

// Returns the offset of the smallest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_min(char **arr, int n) {
	int min_index = 0, i = 1;
	while(i < n) {
		if(intal_compare(arr[i], arr[min_index]) == -1) {
			min_index = i;
		}
		i++;
	}
	return min_index;
}

// Returns the offset of the first occurrence of the key intal in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int intal_search(char **arr, int n, const char* key) {
	for(int i=0; i<n; i++) {
		if(intal_compare(arr[i], key) == 0) {
			return i;
		}
	}
	return -1;
}

// Returns the offset of the first occurrence of the key intal in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int intal_binsearch(char **arr, int n, const char* key) {
	int low=0, high = n-1, mid, cmp, result=-1;
	while(low <= high) {
		mid = low + ((high - low) / 2); 
		cmp = intal_compare(key, arr[mid]);
		if(cmp == 0) {
			result = mid;
			high = mid-1;
		}
		else if(cmp == 1) {
			low = mid + 1;
		}
		else if(cmp == -1) {
			high = mid - 1;
		}
	}
	return result;
}

// Sorts the array of n intals.
// 1 <= n <= 1000
// The implementation should be a O(n log n) algorithm.
void intal_sort(char **arr, int n) {
	merge_sort(arr, 0, n-1);
}

// Coin-Row Problem - Dynamic Programming Solution
// There is a row of n coins whose values are some positive integers C[0..n-1].
// The goal is to pick up the maximum amount of money subject to the constraint that
// no two coins adjacent in the initial row can be picked up.
// 1 <= n <= 1000
// The implementation should be O(n) time and O(1) extra space even though the DP table may be of O(n) size.
// Eg: Coins = [10, 2, 4, 6, 3, 9, 5] returns 25
char* coin_row_problem(char **arr, int n) {
	char *t1, *t2, *t3;
	t1 = (char*)malloc(MAX * sizeof(char));
	strcpy(t1, "0");
	if(n <= 0) {
		return t1;
	}
	t2 = (char*)malloc(MAX * sizeof(char));
	t3 = (char*)malloc(MAX * sizeof(char));
	strcpy(t2, arr[0]);
	for(int i=1; i<n; i++) {
		char *temp = intal_add(arr[i], t1);
		strcpy(t3, temp);
		free(temp);
		strcpy(t1, t2);
		if(intal_compare(t3, t2) == 1) {
			strcpy(t2, t3);
		}
	}
	char *result = intal_copy(t2);
	free(t1);
	free(t2);
	free(t3);
	return result;
}
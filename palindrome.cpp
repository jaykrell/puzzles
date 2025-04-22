#undef NDEBUG
#include <assert.h>
#include <vector> // for testing and brute-force witness only :)
#include <stdio.h>
// The actual problem is to report if a string is a palindrome,
// making at most one removal.
//
// The solution is, like a regular palindrome check (to be described),
// but with a sorta one time fallback. The fallback is to remove
// one character and then check if the result is a palindrome. Now,
// that follows directly from the problem statement. In fact a brute
// force implementation shall be provided for "witness testing".
// But the efficient solution, is, like, to do the removal at the
// point of a mismatch being found. And at that, the try
// removing each of the mismatched pair. It is a little hard to explain,
// but easy to code. Iterating the cases with one removal isn't too awful
// but requires temporary store.

// First a regular palindrome check.
// This is useful as a building block and as part of testing.
// Algorithm is to iterate from each end checking for match,
// until the ends cross. For odd, they will coincide
// and match (eh, we could use < instead of <= therefore). For
// even they will become adjacent and then cross.
//
// Having written the code earlier, we realize it is more
// intuitive to use two pointers than pointer and size.
bool is_palindrome(const char* p, const char* end)
{
	// end is really to an element, not one past.
	// one past is usual idiom, but maybe not ideal here.
	size_t const n = (end - p) + 1;

	// Length 0 or 1 is trivially a palindrome.
	if (n < 2)
		return true;

	while (p < end)
		if (*p++ != *end--)
			return false;
	return true;
}

// Provide an arguably convenient form.
bool is_palindrome(const char* str, size_t n)
{
	// length 0 or 1 is trivially a palindrome
	if (n < 2)
		return true;

	return is_palindrome(str, str + n - 1);
}

// Now we do a brute force approach for testing.

template <typename Lambda>
bool generate_except_one(const char* str, size_t n, const Lambda& lambda)
// Worker for brute force solution, useful for testing.
// Iterate 0 to n removing character position n in str and pass that to callback (lambda).
// This is knowingly very inefficient.
// Some of the use-case is builtin, i.e. iterating until lambda returns true.
// It is not really very efficient, in that it is only generating n cases,
// but it does cost the heap allocation.
{
	// Workspace to build every combination of removing one character.
	std::vector<char> temp(n, 0);

	// The nested loop is indeed O(n^2) as anyone would guess.
	// i.e. copying the string one time O(n) and we are copying n times.

	for (size_t skip = 0; skip < n; ++skip)
	{
		size_t dest = 0;
		for (size_t source = 0; source < n; ++source)
			if (skip != source)
				temp[dest++] = str[source];
		if (lambda(&temp[0], n - 1))
			return true;
	}
	return false;
}

bool is_palindrome_except_one(const char* p, size_t n)
{
	// Length 0 or 1 is trivially a palindrome.
	if (n < 2)
		return true;

	const char* end = (p + n - 1);

	while (p < end) {
		if (*p != *end)
			return is_palindrome(p + 1, end) || is_palindrome(p, end - 1);
		++p;
		--end;
	}
	return true;
}

bool is_palindrome_except_one_slow(const char* str, size_t n)
{
	// Length 0 or 1 is trivially a palindrome.
	if (n < 2)
		return true;

	// First check if removing nothing is a palindrome.
	if (is_palindrome(str, n))
		return true;

	// Iterate as to which character to remove.
	// This is worth a separate test so make it a separate function.
	// Wrap is_palindrome in lambda due to overloading.
	return generate_except_one(str, n, 
		[](const char* str, size_t n) { return is_palindrome(str, n); });
}

int main()
{
	printf("test is_palindrome\n");

	typedef const char* PCSTR;
	struct STR {
		const char* chars;
		size_t size;
	};
#define MAKE_STR(s) {(s), (sizeof(s) - 1)}
	STR data[] = {
		// palindromes
		MAKE_STR("abcba"),
		MAKE_STR("abba"),
		{ },
		// non_palindromes but only require one edit
		MAKE_STR("abcbaa"),
		MAKE_STR("abbab"),
		// non_palindromes, require multiple edits
		// really need more testing and lots of random testing
		MAKE_STR("abcbaaa"),
		MAKE_STR("abbaba"),
	};

	printf("palindromes\n");
	for (auto s: data)
	{
		if (!s.chars) {
			printf("non-palindromes\n");
			continue;
		}
		generate_except_one(s.chars, s.size,
			[&](const char* str, size_t n) {
			printf("generate_except_one(%s):%.*s\n", s.chars, (int)n, str);
		   return false; // keep going
		});

		printf("is_palindrome(%s):%d\n", s.chars, is_palindrome(s.chars, s.size));

		bool except_one_slow = is_palindrome_except_one_slow(s.chars, s.size);
		bool except_one = is_palindrome_except_one(s.chars, s.size);
		assert(except_one_slow == except_one);
		printf("is_palindrome_except_one_slow(%s):%d\n", s.chars, except_one_slow);
		printf("is_palindrome_except_one(%s):%d\n", s.chars, except_one);
	}

	// TODO: A lot more testing.
	// Generate random palindromes, and edit them once,
	// so that, one edit will then restore palindromeness.
}

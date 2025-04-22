#include <stdio.h>
// Given an unordered array of integers, report a local minimum.
// There might be multiple local minima, but any one can be reported.
// A local minimum is:
//   an element where each immediate neighbor is greater or equal
//   That includes potentially the first or last, which only have one neighbor.
//   i.e. Most elements have two immediate neighbor, two elements have one
//   neighbor. It is important remember greater or equal.
//
// One could do:
//  Check each end.
//  Then iterate second to second to last, checking each neighbor.
//  It is a simple linear solution.
//
// However, the key insights:
//  - There is always a local minimum. At least one.
//    If the values are all equal, they are all local minima.
//    If the values go monotonically up, the first is the local minimum.
//    If the values go monotonically down, the last is the local minimum.
//    Otherwise, "what goes down, goes back up", and you just have to find
//    one such point, any valley in a zig zag.
//
// Image a simple example:
//
//  \    /\      /
//   \  /  \    /
//    \/    \__/
//  321012321000123
//
// There are 4 local minima. i.e. all the zeros.
// In this case, the answer is any of the absolute minima. Let's try again
// the more general situation.
//
// 4  \      _     /
// 3   \    / \   /
// 2    \  /   \_/
// 1     \/
// 0
//              1     index/10
//    012345678901234 index
//    432101233211234 value
//
// This is very similar, but you can report the zero or either of the ones.
//
// So again, "what goes down, must go back up" (or stay equal to the end).
//
// Disclosure: I was stuck on the lg(n) solution and interviewer basically
// gave up on me and gave the answer.
//
// It is a little hard to make the leap (again, I didn't), but this means
// that if we look in the middle, we will see 3 elements and either:
//  - The middle is a solution.
//  - Or we go in the direction of the down, away from the up.
//    We go down until we hit bottom, and either across or back up.
//
// Now, it is tempting therefore to just start in the middle and go in one
// direction. This will work. It provides an O(n/2) solution. However
// interviewer asked a very leading question, wanting a lg(n) solution.
//
// If we start in the middle and decide on a direction, can we cut over the
// middle of the correct half? Like, if we go to far toward the down, and hit
// up, just reverse course?
//
// Consider the picture.
// There are 15 values.
// Start at index 7, value 3
// This is a local maximum but that is besides the point.
// Down is the left.
// So we are considering the range [0,7].
// Look at middle index 3, value 1.
// Down is to the right.
// So we are considering the range [3,7]
// Look at the middle index 5, value 1.
// Down is to the left.
// So are considering the range [3,5].
// This is of length 3 and the middle is index 4, value 0, and it is a local
// minimum.
//
// In order to keep the length=3 special, the first step will be to check the
// edges.
//
// This is not the most convincing, "proof by example", but there is a
// vaguely good idea here, of going toward the down, and knowing
// there must be a minimum. Let's code it up.
//
// Return the index, or n for error cases.
// i.e. n=0: error
// n=1: the element is min and max
// n=2: return the min
// n=3: starts to get interesting, any might be the answer
// n=2 can be absorbed into checking the edges if they are the answer,
// so that later logic can avoid bounds checks.
//
// Spec is int but we have chars because of our input data.
//
size_t local_minimum(const char *data, size_t n) {
  size_t local_minimum_helper(const char *data, size_t start, size_t end);

  if (n == 0)
    return 0;
  // length 1 means the first is the answer
  if (n == 1)
    return 0;
  // Check the first element.
  if (data[0] <= data[1])
    return 0;
  // Check the last element.
  if (data[n - 1] <= data[n - 2])
    return n - 1;
  // Now every element has two neighbors, without bounds checking.
  return local_minimum_helper(data, 0, n);
}

size_t local_minimum_helper(const char *data, size_t start, size_t end) {
  size_t middle = start + (end - start) / 2;
  if (data[middle - 1] >= data[middle] && data[middle] <= data[middle + 1])
    return middle;
  if (data[middle - 1] <= data[middle])
    return local_minimum_helper(data, start, middle);
  return local_minimum_helper(data, middle, end);
}

int main() {
  printf("test local_minimum\n");
  printf("01234567890123456 index\n");

  struct STR {
    const char *chars;
    size_t size;
  };
#define MAKE_STR(s) {(s), (sizeof(s) - 1)}
  STR data[] = {
      MAKE_STR("432101233211234"),
      MAKE_STR("321012321000123"),
      MAKE_STR("A"),
      MAKE_STR("12"),
      MAKE_STR("21"),
      MAKE_STR("123456789"),
      MAKE_STR("987654321"),
  };

  for (auto s : data) {
    int answer = (int)local_minimum(s.chars, s.size);
    printf("%s[%d]:%c\n", s.chars, answer, s.chars[answer]);
  }

  // TODO: A lot more testing.
}

#ifndef TASSERT_H
#define TASSERT_H
#ifndef FULL_FAT_ASSERT
# include <stdlib.h>
# define assert(expr) do { if (!(expr)) abort(); } while (0)
#else
# include <assert.h>
#endif

#endif

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
    int a[4] = {1,2,3,4};
    for(int i=0; i<10; i++)
        hellobhai(a);
    exit();
}

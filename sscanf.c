#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[])
{
    char *str = "ALARM 2016-12-08#11:14:01";
    char str1[6];
    int yy;
    int mm;
    int dd;
    int h;
    int m1;
    int s;
    sscanf(str, "%5s %4d-%2d-%2d#%2d:%2d:%2d",str1, &yy,&mm,&dd,&h,&m1,&s);
    printf("%5s %04d-%02d-%02d#%02d:%02d:%02d\n",str1, yy,mm,dd,h,m1,s);


    //아래는 참고만
    char *src = "ABC|DEF%GHI&JKL@123 MNO_PQR/STU[VWX]YZ 가나다";
   char dest1[50],dest2[50], dest3[50], dest4[50], dest5[50], dest6[50];
   int num = 0, m=0, n=0;
   char *param = "id=myid&pwd=mypwd&name=smith";
   char *param_num = "m=100&n=5";

   /* 공백으로 구분된 첫번째 문자열 전체*/
   sscanf(src, "%s", dest1); // ABC|DEF%GHI&JKL@123
   printf("1. %s \n", dest1);

   /* 공백으로 구분된 첫번째 문자열 중에서 '&'바로 앞까지의 부분 문자열 */
   sscanf(src, "%[^&]", dest1); // ABC|DEF%GHI
   printf("2. %s \n", dest1);

   sscanf(src, "%[^F]", dest1); // ABC|DE
   printf("3. %s \n", dest1);

   /* %*s 표현의 '*'기호는 skip(건너뜀)의 의미이며 's'는 공백으로 구분된 문자열을 의미하므로
   공백으로 구분된 한개의 문자열은 읽어들이지 말고 건너 뛰라는 의미이다
   두번째 문자열을 읽어온다*/
   sscanf(src, "%*s%s", dest1); // MNO_PQR/STU[VWX]YZ
   printf("4. %s \n", dest1);

   /* '@'왼쪽까지 한개의 문자열로 읽어들이고, '@'는 두번째 문자열로 읽어들이고,
   '@'우측에서부터 공백이 나올 때까지는 정수로 읽어들인다 */
   sscanf(src, "%[^@]%[@]%d", dest1, dest2, &num);
   printf("5. %d \n", num); // 123

   num = 0;
   sscanf(src, "%[^@]%*[@]%d", dest1, &num); // '@' 문자는 읽지 않음 */
   printf("6. %d \n", num); // 123

   // 공백으로 구분된 문자열 2개를 건너뛰고 3번째 문자열을 읽어 들이라는 의미
   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%*s%*s%s", dest1);
   printf("7. %s \n", dest1); // 가나다

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%*[A-F,|,%]%[^&]", dest1); // GHI
   printf("8. %s \n", dest1);

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%[F]", dest1);
   printf("9. %s \n", dest1); // 실패

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%[A-Z]", dest1);
   printf("10.%s \n", dest1); // ABC

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%[C]", dest1);
   printf("11.%s \n", dest1); // 실패

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%[D]", dest1);
   printf("12.%s \n", dest1); // 실패

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%[^|]", dest1);//ABC
   printf("13.%s \n", dest1);

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%*[A-C,|]%[^\%]", dest1);// DEF
   printf("14.%s \n", dest1);

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%*[^&]%*[&]%[^@]", dest1); //JKL
   printf("15.%s \n", dest1);

   memset(dest1,0,sizeof(dest1));
   sscanf(src, "%*[^0-9]%d", &num); //123
   printf("16.%d \n", num);

   memset(dest1, 0, sizeof(dest1));
   sscanf(src, "%*s%*[^[]%*[[]%[^]]", dest1); // VWX
   printf("17.%s \n", dest1);

   memset(dest1, 0, sizeof(dest1));
   memset(dest2, 0, sizeof(dest2));
   memset(dest3, 0, sizeof(dest4));

   sscanf(param, "%[^&]%*[&]%[^&]%*[&]%s", dest1, dest2, dest3);// id=myid, pwd=mypwd, name=smith
   printf("18.%s, %s, %s \n", dest1, dest2, dest3);

   memset(dest1, 0, sizeof(dest1));
   memset(dest2, 0, sizeof(dest2));
   memset(dest3, 0, sizeof(dest4));

   /* 아래와 같이 간단하게 파라미터를 읽어오려고 하지만 되지않는 이유는
   * 처음에 id=%s 부분에서 %s가 의미하는 것은 전체문자열이기 때문에
   * 제대로 읽어 올 수가 없다 */
   sscanf("id=%s&pwd=%s&name=%s", dest1, dest2, dest3); // 실패
   printf("19.%s, %s, %s \n", dest1, dest2, dest3);

   memset(dest1, 0, sizeof(dest1));
   memset(dest2, 0, sizeof(dest2));
   memset(dest3, 0, sizeof(dest4));

   /* 파라미터 값이 모두 숫자로만 구성되어 있다면 보다 쉽게 읽어올 수 있다
   * 이 방법이 가능한 이유는 m=%d 에서 %d는 숫자이기 때문에 다른 문자와 구별이 되므로
   * 위에서와 같은 전체 문자열을 읽어오는 경우에 해당하지 않기 때문이다 */
   sscanf(param_num, "m=%d&n=%d", &m, &n);
   printf("20.%d, %d \n", m, n); // 100, 5

    return 0;
}

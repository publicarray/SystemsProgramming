void adjustr (char *s, int count)
{
    int n;
    for (n = 0; s[n + count] != '\0'; ++n){
        s[n] = s[n + count];
    }
    s[n] = '\0';
}

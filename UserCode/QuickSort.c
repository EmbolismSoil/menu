void QuickSort(double s[], int Left, int Right)
{
    int cntLeft, cntRight, ref;
    if (Left < Right)
    {
        cntLeft = Left;
        cntRight = Right;
        ref = s[cntLeft];
        while (cntLeft < cntRight)
        {
            while(cntLeft < cntRight && s[cntRight] > ref) cntRight--; /* 从右向左找第一个小于x的数 */
            if(cntLeft < cntRight) s[cntLeft++] = s[cntRight];
            while(cntLeft < cntRight && s[cntLeft] < ref) cntLeft++; /* 从左向右找第一个大于x的数 */
            if(cntLeft < cntRight) s[cntRight--] = s[cntLeft];
        }
        s[cntLeft] = ref;
        QuickSort(s, Left, cntLeft - 1); /* 递归调用 */
        QuickSort(s, cntLeft+1, Right);
    }
}
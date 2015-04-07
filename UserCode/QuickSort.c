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
            while(cntLeft < cntRight && s[cntRight] > ref) cntRight--; /* ���������ҵ�һ��С��x���� */
            if(cntLeft < cntRight) s[cntLeft++] = s[cntRight];
            while(cntLeft < cntRight && s[cntLeft] < ref) cntLeft++; /* ���������ҵ�һ������x���� */
            if(cntLeft < cntRight) s[cntRight--] = s[cntLeft];
        }
        s[cntLeft] = ref;
        QuickSort(s, Left, cntLeft - 1); /* �ݹ���� */
        QuickSort(s, cntLeft+1, Right);
    }
}
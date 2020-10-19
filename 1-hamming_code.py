"""
'a' = 0
 ~
'z' = 25

' ' = 26
'.' = 27

合計28文字のハミング距離が3以上になるようにそれぞれの検査ビットを設計
　→ 4 bit に変更
"""

import pandas as pd


def count_diff(a, b):
    return sum(
        c1 != c2 for c1, c2 in zip(a, b)
    )

def hamming_4(num):
    x1 = int(num[0])
    x2 = int(num[1])
    x3 = int(num[2])
    x4 = int(num[3])

    c1 = (x1 + x2 + x3) % 2
    c2 = (x1 + x3 + x4) % 2
    c3 = (x2 + x3 + x4) % 2

    return f'{x1}{x2}{x3}{x4}{c1}{c2}{c3}'


def main():
    maxvalue = 15
    maxsize = len(bin(maxvalue)) - 2
    
    df = pd.DataFrame(index=range(0, maxvalue+1), columns=range(0, maxvalue+1))

    df.index = list(map(lambda x: format(int(x), 'b').zfill(maxsize), df.index.tolist()))
    df.columns= list(map(lambda x: format(int(x), 'b').zfill(maxsize), df.columns.tolist()))

    # 初期状態の距離を確認
    for col in df.columns:
        for idx in df.index:
            if idx == col:
                break

            df.loc[idx, col] = count_diff(idx, col)


    print("\n=== 元のデータ")
    print(df)


    hamminged_lst = list(map(lambda x: hamming_4(x), df.index.tolist()))

    df.index = hamminged_lst
    df.columns= hamminged_lst


    # 　最終的な距離の確認
    for col in df.columns:
        for idx in df.index:
            if idx == col:
                break

            df.loc[idx, col] = count_diff(idx, col)

    print("\n=== ハミング符号を付与したデータ")
    print(df)
                 



if __name__ == "__main__":
    main()

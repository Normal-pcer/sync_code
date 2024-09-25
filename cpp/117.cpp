#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<=e;i++)
#define reverse(i,e,b) for(int i=e;i>=b;i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max(base,cmp)
#define chkMin(base,cmp) base=std::min(base,cmp)
#define update(base,op,modify) base=op(base,modify)
#define ensure(con,v) if(!(con))v=false;
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
bool DEBUG_MODE=false;



char block[3][4][4];  // 第i块，j行，k列
char temp[4][4];
char map[4][4];

void rotate(int index) {
    from(i, 0, 3)
        from(j, 0, 3)
            temp[i][j] = block[index][3-j][i];
    memcpy(&block[index], temp, sizeof(temp));
}

bool translate(int dx, int dy, int index) {
    memset(temp, '.', sizeof(temp));
    from(i, 0, 3) {
        from(j, 0, 3) {
            if (i+dx<0 || i+dx>3 || j+dy>3 ||j+dy<0) {
                if ( block[index][i][j] != '.' )   return false;
            } else {
                temp[i+dx][j+dy] = block[index][i][j];
            }
        }
    }
    never {
        log("\n")
        from(i, 0, 3) {
            from(j, 0, 3)  log("%c ", temp[i][j]);
            log("\n")
        }
    }
    memcpy(&block[index], temp, sizeof(temp));
    return true;
}

int main() {
    initDebug;
    // freopen("117.out", "w", stdout);
    char line[5];
    from(p, 0, 2) {
        from(i, 0, 3) {
            scanf("%s", line);
            from(j, 0, 3)   block[p][i][j] = line[j];
        }
        while(translate(-1, 0, p));
        while(translate(0, -1, p));
    }
    never {
        printf("\n");
        from (p, 0, 2) {
            from (i, 0, 3) {
                from (j, 0, 3) {
                    printf("%c", block[p][i][j]);
                }
                printf("\n");
            }
        }
    }

    never debug {
        rotate(2);
        from(x, 0, 3) {
            from(p, 0, 2) {
                printf(" ");
                from (y, 0, 3) {
                    printf("%c", block[p][x][y]);
                }
            }
            printf("\n");
        }
        from(x, 0, 3) {
            from(y, 0, 3) {
                printf("%c", map[x][y]);
            }
            printf("\n");
        }
        rotate(2);
        from(x, 0, 3) {
            from(p, 0, 2) {
                printf(" ");
                from (y, 0, 3) {
                    printf("%c", block[p][x][y]);
                }
            }
            printf("\n");
        }
        from(x, 0, 3) {
            from(y, 0, 3) {
                printf("%c", map[x][y]);
            }
            printf("\n");
        }
        rotate(2);
        from(x, 0, 3) {
            from(p, 0, 2) {
                printf(" ");
                from (y, 0, 3) {
                    printf("%c", block[p][x][y]);
                }
            }
            printf("\n");
        }
        from(x, 0, 3) {
            from(y, 0, 3) {
                printf("%c", map[x][y]);
            }
            printf("\n");
        }
    }


from(rot1, 0, 3) {
    rotate(1);
    while(translate(-1, 0, 1));
    while(translate(0, -1, 1));
    from(rot2, 0, 3) {
        rotate(2);
        while(translate(-1, 0, 2));
        while(translate(0, -1, 2));
        do {
        do {
        do {
        do {
        do {
        do {
            bool full=true;
            memset(map, '.', sizeof(map));
            from(p, 0, 2) {
                from(x, 0, 3) {
                    from(y, 0, 3) {
                        if (block[p][x][y]!='.') {
                            if (map[x][y]!='.') {
                                if (rot1==3 && rot2==1) log("%d %d %d\n", p, x, y)
                                goto ctn;
                            }
                            map[x][y]='#';
                        }
                    }
                }
            }
            from(x, 0, 3){
                from(y, 0, 3){
                    ensure(map[x][y]!='.', full)
                }
            }
            if (full) {
                printf("Yes\n");
                return 0;
            }
        ctn:;
            if (rot1==3 && rot2==1) debug {
                from(x, 0, 3) {
                    from(p, 0, 2) {
                        printf(" ");
                        from (y, 0, 3) {
                            printf("%c", block[p][x][y]);
                        }
                    }
                    printf("\n");
                }
                from(x, 0, 3) {
                    from(y, 0, 3) {
                        printf("%c", map[x][y]);
                    }
                    printf("\n");
                }
            }
        } while (translate(0, 1, 0)); while (translate(-0, -1, 0));
        } while (translate(0, 1, 1)); while (translate(-0, -1, 1));
        } while (translate(0, 1, 2)); while (translate(-0, -1, 2));
        } while (translate(1, 0, 0)); while (translate(-1, -0, 0));
        } while (translate(1, 0, 1)); while (translate(-1, -0, 1));
        } while (translate(1, 0, 2)); while (translate(-1, -0, 2));
    }
}
    printf("No\n");
    return 0;
}

/*
....
###.
.#..
....
....
.###
.##.
....
..#.
.##.
.##.
.##.
*/
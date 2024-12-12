/*
https://www.luogu.com.cn/discuss/795990
主猪MP
忠猪ZP
反猪FP
『桃 /P』
『闪 /D』
『杀 /K』
『决斗 /F』
『南蛮入侵 /N』
『万箭齐发 /W』
『无懈可击 /J』
『诸葛连弩 /Z』
*/
#include<bits/stdc++.h>
#include "./lib"


using std::string;
using std::vector;
using std::cin;
using std::cout;

enum character {
    MP = 'M',
    ZP = 'Z',
    FP = 'F'
};
int Begin = 1, End, cnt_of_FP, n, m, now_i, now_ii;
string winner;
vector<char> heap;
class account {
public:
    character ch = MP;
    vector<char> cards;
    int blood = 4;                      // 生命值
    int index = 0;                      // 当前位置
    int ans_index = 0;                  // 初始位置
    int jumped = 0;                     // 1 = 跳忠，2 = 跳反
    int similar_jumped = 0;             // 包含“类反猪”的判定
    bool infinite_kill = 0;             // 猪哥连弩
    bool this_turn_kill = 1;            // 使用过“杀”
    void choose_card(int num);          // 抽卡
    bool check_card_valid(char use_card, account enemy);  // 检查合法性

    int find_enemy(char use_card);  // 返回敌人的 index
    int death();                    // 是否死亡，如果是则进行后续工作
    void kill(account& enemy);      // 使用“杀”
    void fight(account& enemy);     // 发起决斗
    void arrows();                  // 万箭齐发
    void invade();                  // 南猪入侵
    bool peach();                   // 桃

    std::string identity() const {
        return std::format("#{}", ans_index);
    }

    operator std::string () const {
        auto base = std::format("Account(ch={}, HP={}, index={}, origin={}, jumped={}, similar={}, inf_kill={}, used_kill={}) with cards:\n",
            (char)ch, blood, index, ans_index, jumped, similar_jumped, infinite_kill, this_turn_kill
        );
        std::stringstream stream;
        for (auto c: cards)  stream << c << ' ';
        stream << '\n';
        return base + stream.str();
    }

    friend std::ostream &operator<<(std::ostream &stream, account const &element) {
        stream << (std::string)element;
        return stream;
    }
};
vector<account> acc;
void GAMEOVER() {
    cout<<winner<<endl;
    int pre = 1;
    for (int i = Begin;i<=End;i++) {
        for (;pre<acc[i].ans_index;pre++)cout<<"DEAD"<<endl;
        for (auto j = acc[i].cards.begin()+1;j<acc[i].cards.end()-1;j++) {
            cout<<*j<<" ";
        }
        if (acc[i].cards.size()>1)cout<<*(acc[i].cards.end()-1);
        pre = acc[i].ans_index+1;
        cout<<endl;
    }
    for (int i = acc[End].ans_index+1;i<=n;i++)cout<<"DEAD"<<endl;
    throw 0;
}
void things_after_death(account& subject, account& object) {
    if (!object.death())return;
    else {
        if (object.index<=now_i)now_i--;
        switch (object.ch) {
        case MP:
            winner = "FP";
            for (int i = 2;i<=End;i++) {
                acc[i].index--;
            }
            End--;
            acc.erase(acc.begin()+1);
            GAMEOVER();
            break;
        case ZP:
            if (subject.ch==MP) {
                while (subject.cards.size()>1) {
                    subject.cards.erase(subject.cards.begin()+1);
                }
                subject.infinite_kill = 0;
            }
            for (int i = object.index+1;i<=End;i++) {
                acc[i].index--;
            }
            End--;
            acc.erase(acc.begin()+object.index);
            break;
        case FP:
            cnt_of_FP--;
            if (cnt_of_FP==0) {
                winner = "MP";
                for (int i = object.index+1;i<=End;i++) {
                    acc[i].index--;
                }
                End--;
                acc.erase(acc.begin()+object.index);
                GAMEOVER();
            } else {
                for (int i = object.index+1;i<=End;i++) {
                    acc[i].index--;
                }
                End--;
                acc.erase(acc.begin()+object.index);
                subject.choose_card(3);
            }
            break;
        }
    }
}
bool account::check_card_valid(char use_card, account enemy) {
    int idx = index;
    int right = idx+1;
    if (idx==End)right = Begin;
    switch (use_card) {
    case 'K':
        return enemy.index==right;
        break;
    case 'F':
        return enemy.index!=index;
        break;
    default:
        return 0;
    }
}
int account::find_enemy(char use_card) {
    int idx = index;
    int right = idx+1;
    if (idx==End)right = Begin;
    for (int i = right;i!=idx;i++) {
        switch (ch) {
        case MP:
            if (acc[i].jumped) {
                if (acc[i].jumped==2) {
                    if (check_card_valid(use_card, acc[i]))return i;
                }
            } else {
                if (acc[i].similar_jumped) {
                    if (check_card_valid(use_card, acc[i]))return i;
                }
            }
            break;
        case ZP:
            if (acc[i].jumped) {
                if (acc[i].jumped==2) {
                    if (check_card_valid(use_card, acc[i]))return i;
                }
            }
            break;
        case FP:
            if (check_card_valid(use_card, acc[1]))return 1;
            if (acc[i].jumped) {
                if (acc[i].jumped==1||acc[i].jumped==3) {
                    if (check_card_valid(use_card, acc[i]))return i;
                }
            }
            break;
        }
        if (i==End) {
            i = Begin-1;
        }
    }
    return 0;
}
void account::choose_card(int num) {
    for (int i = 1;i<=num;i++) {
        cards.push_back(heap[0]);
        if (heap.size()>1)
            heap.erase(heap.begin());
    }
}
int account::death() {
    if (blood>0)return 0;
    else {
        auto it = std::find(cards.begin(), cards.end(), 'P');
        if (it!=cards.end()) {
            cards.erase(it);
            blood++;
            //			cout<<index<<" peach itself "<<endl;
            return 0;
        } else return 1;
    }
}
void account::kill(account& enemy) {
    debug  std::cout << std::format("{} Use Kill to {}", identity(), enemy.identity()) << std::endl;
    auto it1 = std::find(cards.begin(), cards.end(), 'K');
    cards.erase(it1);
    auto it = std::find(enemy.cards.begin(), enemy.cards.end(), 'D');
    if (it!=enemy.cards.end()) {
        enemy.cards.erase(it);
    } else {
        enemy.blood--;
    }
    if (enemy.jumped&&!jumped) {
        if (enemy.jumped==1||enemy.jumped==3)jumped = 2;
        if (enemy.jumped==2)jumped = 1;
    }

    things_after_death(acc[index], enemy);
}
bool check_skill_valid(account& pos, account& used_for, bool check) {

    int idx = pos.index;
    int loop = 0;
    for (int i = idx;loop<=2;i++) {
        //		cout<<acc[i].index<<" "<<acc[i].cards.size()<<endl;
        if (!used_for.jumped&&!used_for.similar_jumped)return true;
        auto it = std::find(acc[i].cards.begin(), acc[i].cards.end(), 'J');
        if (it!=acc[i].cards.end()) {
            switch (acc[i].ch) {
            case MP:
                if (check) {
                    if (used_for.jumped==2) {
                        acc[i].cards.erase(it);
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                    if (!used_for.jumped&&used_for.similar_jumped) {
                        acc[i].cards.erase(it);
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                } else {
                    if (used_for.jumped==1||used_for.jumped==3) {
                        acc[i].cards.erase(it);
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                }

                break;
            case ZP:
                if (check) {
                    if (used_for.jumped==2) {
                        acc[i].cards.erase(it);
                        acc[i].jumped = 1;
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                } else {
                    if (used_for.jumped==3||used_for.jumped==1) {
                        acc[i].cards.erase(it);
                        acc[i].jumped = 1;
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                }

                break;
            case FP:
                if (check) {
                    if (used_for.jumped==3||used_for.jumped==1) {
                        acc[i].cards.erase(it);
                        acc[i].jumped = 2;
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                }
                if (used_for.jumped==3||used_for.jumped==1) {
                    if (used_for.jumped==2) {
                        acc[i].cards.erase(it);
                        acc[i].jumped = 2;
                        return !check_skill_valid(acc[i], acc[i], 1);
                    }
                }
                break;
            }
        }

        if (i==idx) {
            loop++;
        }
        if (i==End) {
            i = Begin-1;
        }
    }
    return true;
}
void account::fight(account& enemy) {
    debug  std::cout << std::format("{} Use Fight to {}", identity(), enemy.identity()) << std::endl;
    auto it1 = std::find(cards.begin(), cards.end(), 'F');
    cards.erase(it1);
    if (enemy.ch==MP)jumped = 2;
    if (enemy.ch==FP&&!jumped)jumped = 1;
    if (check_skill_valid(acc[index], enemy, 0)) {
        if (enemy.ch==ZP&&ch==MP) {
            enemy.blood--;
            things_after_death(acc[index], enemy);
        } else {
            while (1) {
                auto it = std::find(enemy.cards.begin(), enemy.cards.end(), 'K');
                if (it==enemy.cards.end()) {
                    //					cout<<index<<" fight "<<enemy.index<<endl;
                    enemy.blood--;
                    things_after_death(acc[index], enemy);
                    break;
                } else {
                    if (ch==MP&&enemy.ch==FP)enemy.jumped = 2;
                    enemy.cards.erase(it);
                    auto it2 = std::find(cards.begin(), cards.end(), 'K');
                    if (it2==cards.end()) {
                        blood--;
                        things_after_death(enemy, acc[index]);
                        break;
                    } else {
                        cards.erase(it2);
                    }
                }
            }
        }
    }
    //	else cout<<"无懈可击\n";
}
void account::arrows() {
    debug  std::cout << std::format("{} Use Arrows", identity()) << std::endl;
    auto it1 = std::find(cards.begin(), cards.end(), 'W');
    cards.erase(it1);
    int idx = index;
    int right = idx+1;
    if (idx==End)right = Begin;
    for (int i = right;i!=index;i++) {
        //		cout<<i<<" "<<endl;
        if (!check_skill_valid(acc[index], acc[i], 0))break;
        else {
            auto it = std::find(acc[i].cards.begin(), acc[i].cards.end(), 'D');
            if (it==acc[i].cards.end()) {
                acc[i].blood--;
                if (i==Begin)similar_jumped = 1;
                if (acc[i].death()) {
                    if (i<index) {
                        i--;
                        things_after_death(acc[index], acc[i+1]);
                    }
                    things_after_death(acc[index], acc[i]);

                }

            } else {
                acc[i].cards.erase(it);
            }
        }
        if (i==End) {
            i = Begin-1;
        }
    }
}
void account::invade() {
    debug  std::cout << std::format("{} Use Invade", identity()) << std::endl;
    auto it1 = std::find(cards.begin(), cards.end(), 'N');
    cards.erase(it1);
    int idx = index;
    int right = idx+1;
    if (idx==End)right = Begin;
    for (int i = right;i!=index;i++) {
        if (!check_skill_valid(acc[index], acc[i], 0))break;
        else {
            auto it = std::find(acc[i].cards.begin(), acc[i].cards.end(), 'K');
            if (it==acc[i].cards.end()) {
                if (i==Begin)similar_jumped = 1;
                acc[i].blood--;
                if (acc[i].death()) {
                    if (i<index) {
                        i--;
                        things_after_death(acc[index], acc[i+1]);
                    }
                    things_after_death(acc[index], acc[i]);
                }
            } else {
                acc[i].cards.erase(it);
            }
        }
        if (i==End) {
            i = Begin-1;
        }
    }
}
bool account::peach() {
    debug  std::cout << std::format("{} Use Peach", identity()) << std::endl;
    if (blood!=4) {
        blood++;
        return true;
    }
    return false;
}
// 玩家行动轮
void play(account& ac) {
    ac.choose_card(2);  // 摸牌
    for (auto it = ac.cards.begin(); it < ac.cards.end(); it++) {
        switch (*it) {
        case 'P':
            if (ac.peach()) {
                //					cout<<ac.index<<" peach ";
                ac.cards.erase(it);
                it = ac.cards.begin();
            }
            break;
        case 'K':
            if (ac.infinite_kill) {
                if (ac.find_enemy('K')) {
                    //						cout<<ac.index<<" kill "<< ac.find_enemy('K')<<endl;
                    ac.kill(acc[ac.find_enemy('K')]);
                    it = ac.cards.begin();
                }
            } else if (ac.this_turn_kill) {
                if (ac.find_enemy('K')) {
                    //						cout<<ac.index<<" kill "<< ac.find_enemy('K')<<endl;
                    ac.kill(acc[ac.find_enemy('K')]);
                    it = ac.cards.begin();
                    ac.this_turn_kill = 0;
                }
            }
            break;
        case 'F':
            if (ac.find_enemy('F')) {
                //					cout<<ac.index<<" fight "<< ac.find_enemy('F')<<endl;
                ac.fight(acc[ac.find_enemy('F')]);
                it = ac.cards.begin();
            }
            break;
        case 'N':
            ac.invade();
            //				cout<<ac.cards.size()<<endl;
            it = ac.cards.begin();
            break;
        case 'W':
            ac.arrows();
            it = ac.cards.begin();
            break;
        case 'Z':
            ac.infinite_kill = 1;
            ac.cards.erase(it);
            it = ac.cards.begin();
            break;
        }
    }
}
int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    //	freopen("D:\\in.txt","r",stdin); //输入重定向，输入数据将从D盘根目录下的in.txt文件中读取 
    //	freopen("D:\\out.txt","w",stdout); //输出重定向，输出数据将保存在D盘根目录下的out.txt文件中 

    cin>>n>>m;  // 输入 n 和 m
    End = n;  // 将变量 End 赋值为 n
    //	
    account empty_acc;  // 定义一个类型为 account 的变量，名为 empty_acc
    acc.push_back(empty_acc);  // 
    for (int i = 1;i<=n;i++) {
        account pig;
        pig.cards.push_back('A');
        pig.ans_index = pig.index = i;
        string a0;
        cin>>a0;
        switch (a0[0]) {
        case 'M':
            pig.ch = MP;
            break;
        case 'Z':
            pig.ch = ZP;
            break;
        case 'F':
            pig.ch = FP;
            cnt_of_FP++;
            break;
        }
        for (int j = 1;j<=4;j++) {
            char c0;
            cin>>c0;
            pig.cards.push_back(c0);
        }
        acc.push_back(pig);
    }
    for (int i = 1;i<=m;i++) {
        char c;
        cin>>c;
        heap.push_back(c);
    }
    acc[1].jumped = 3;
    if (cnt_of_FP==0) {
        winner = "MP";
        GAMEOVER();
    }
    try {
        while (1) {
            for (int i = 1;;i++) {
                now_i = i;
                debug  std::cout << acc[i];
                play(acc[i]);
                i = now_i;
                //			cout<<i<<" "<<acc[1].blood<<endl;
                //			for(auto j:acc[i].cards)cout<<j;
                //			cout<<endl;
                acc[i].this_turn_kill = 1;
                if (i==End)i = 0;
            }
        }
    } catch (int err) {
        if (err == 0)  return 0;
        else  throw err;
    }
    //	acc[2].invade();
    //
    //	acc[1].fight(acc[2]);

}
/*
FP
DEAD
K P P
K

W F P
*/
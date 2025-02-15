#include <bits/stdc++.h>
using namespace std;

struct Player {
    string role;
    int hp, max_hp;
    vector<char> hand;
    bool has_zgln;
    bool dead;
    bool is_zjump; // 跳忠
    bool is_fjump; // 跳反
    bool is_leifan; // 是否被主猪认为是类反猪
    int id;

    Player() {}
    Player(string r, int id) : role(r), hp(4), max_hp(4), has_zgln(false), dead(false), is_zjump(false), is_fjump(false), is_leifan(false), id(id) {}

    bool can_use_peach() {
        return hp < max_hp;
    }
};

vector<Player> players;
vector<char> pile;
char last_card = 0;
int n, m;
bool game_over = false;
string winner;

int current_player_idx;

// 计算存活玩家的距离，逆时针方向间隔+1
int calc_distance(int from, int to, const vector<bool>& alive) {
    if (from == to) return 0;
    int step = 0;
    for (int i = (from + 1) % n; ; i = (i + 1) % n) {
        if (alive[i]) step++;
        if (i == to) return step;
    }
}

// 寻找下一个存活玩家
int next_player(int idx, const vector<bool>& alive) {
    int next = (idx + 1) % n;
    while (next != idx) {
        if (alive[next]) return next;
        next = (next + 1) % n;
    }
    return -1;
}

// 处理玩家死亡
void handle_death(int dead_id, int source_id) {
    Player& dead = players[dead_id];
    dead.dead = true;
    dead.hand.clear();
    dead.has_zgln = false;

    // 处理奖励与惩罚
    if (dead.role == "FP") {
        // 反猪死亡，source摸3张
        if (!game_over && source_id != -1) {
            for (int i = 0; i < 3; ++i) {
                char c = last_card;
                if (!pile.empty()) {
                    c = pile[0];
                    pile.erase(pile.begin());
                }
                players[source_id].hand.push_back(c);
            }
        }
    } else if (dead.role == "ZP") {
        // 忠猪死亡，source是主猪的话，主猪弃所有牌
        if (source_id != -1 && players[source_id].role == "MP") {
            players[source_id].hand.clear();
            players[source_id].has_zgln = false;
        }
    }

    // 检查胜利条件
    bool has_fp = false;
    bool has_mp = players[0].dead ? false : true;
    for (const auto& p : players) {
        if (p.dead) continue;
        if (p.role == "FP") has_fp = true;
    }
    if (!has_fp && has_mp) {
        game_over = true;
        winner = "MP";
        return;
    }
    if (!has_mp) {
        game_over = true;
        winner = "FP";
        return;
    }
}

// 处理使用无懈可击
bool check_wuxiekeji(int user_id, int target_id, char card_type, int source_id) {
    vector<bool> alive(n);
    for (int i = 0; i < n; ++i) alive[i] = !players[i].dead;

    // 从source开始逆时针询问
    for (int i = source_id; ; i = (i + 1) % n) {
        if (i == source_id && !alive[i]) continue;
        if (!alive[i]) continue;

        Player& p = players[i];
        bool can_use = false;
        // 判断是否可以使用无懈可击
        // 需要判断身份是否允许献殷勤或表敌意
        if (card_type == 'F') {
            // 决斗的无懈可击，根据目标判断
            if (target_id == i) {
                // 被决斗的目标是否需要使用无懈可击？
                // 原来源是决斗的使用者，这里可能需要重新分析
                // 假设这里需要判断是否可以响应
            }
        }

        // 简化逻辑：如果手中有J，并且符合身份行为准则，则使用
        auto it = find(p.hand.begin(), p.hand.end(), 'J');
        if (it != p.hand.end()) {
            // 暂时假设使用，后续需要根据身份逻辑判断
            p.hand.erase(it);
            // 确定是献殷勤还是表敌意，这里简化处理
            return true;
        }

        if (i == (source_id - 1 + n) % n) break;
    }
    return false;
}

// 使用决斗
void use_duel(int user_id, int target_id) {
    Player& user = players[user_id];
    Player& target = players[target_id];

    bool can_wuxie = check_wuxiekeji(user_id, target_id, 'F', user_id);
    if (can_wuxie) {
        return;
    }

    // 决斗处理
    bool user_turn = false;
    while (true) {
        if (!user_turn) {
            // 目标出杀
            auto it = find(target.hand.begin(), target.hand.end(), 'K');
            if (it == target.hand.end()) {
                target.hp -= 1;
                if (target.hp <= 0) {
                    handle_death(target_id, user_id);
                }
                break;
            } else {
                target.hand.erase(it);
            }
        } else {
            // 用户出杀
            auto it = find(user.hand.begin(), user.hand.end(), 'K');
            if (it == user.hand.end()) {
                user.hp -= 1;
                if (user.hp <= 0) {
                    handle_death(user_id, target_id);
                }
                break;
            } else {
                user.hand.erase(it);
            }
        }
        user_turn = !user_turn;
    }
}

// 使用南猪入侵或万箭齐发
void use_aoe(int user_id, char type) {
    vector<bool> alive(n);
    for (int i = 0; i < n; ++i) alive[i] = !players[i].dead;

    int current = (user_id + 1) % n;
    while (current != user_id) {
        if (!alive[current]) {
            current = (current + 1) % n;
            continue;
        }

        Player& p = players[current];
        bool need_discard = (type == 'N') ? 'K' : 'D';
        auto it = find(p.hand.begin(), p.hand.end(), need_discard);
        bool has_card = it != p.hand.end();

        bool can_wuxie = check_wuxiekeji(user_id, current, type, user_id);
        if (can_wuxie) {
            has_card = false;
        }

        if (!has_card) {
            p.hp -= 1;
            if (p.hp <= 0) {
                handle_death(current, user_id);
                if (game_over) return;
            }
        } else {
            p.hand.erase(it);
        }

        current = (current + 1) % n;
    }
}

int main() {
    cin >> n >> m;
    players.resize(n);
    for (int i = 0; i < n; ++i) {
        string role;
        vector<char> hand(4);
        cin >> role;
        for (int j = 0; j < 4; ++j) {
            cin >> hand[j];
        }
        players[i] = Player(role, i+1);
        players[i].hand = hand;
    }

    pile.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> pile[i];
    }
    if (!pile.empty()) last_card = pile.back();

    current_player_idx = 0; // 主猪是第一个玩家

    vector<bool> alive(n, true);

    while (!game_over) {
        if (players[current_player_idx].dead) {
            current_player_idx = (current_player_idx + 1) % n;
            continue;
        }

        Player& current = players[current_player_idx];

        // 摸牌阶段
        for (int i = 0; i < 2; ++i) {
            if (pile.empty()) {
                current.hand.push_back(last_card);
            } else {
                char c = pile[0];
                pile.erase(pile.begin());
                current.hand.push_back(c);
                if (pile.empty()) last_card = c;
            }
        }

        // 出牌阶段
        bool used_kill = false;
        while (true) {
            bool card_used = false;
            for (size_t i = 0; i < current.hand.size(); ++i) {
                char card = current.hand[i];
                if (card == 'P' && current.can_use_peach()) {
                    current.hp += 1;
                    current.hand.erase(current.hand.begin() + i);
                    card_used = true;
                    break;
                } else if (card == 'Z') {
                    current.has_zgln = true;
                    current.hand.erase(current.hand.begin() + i);
                    card_used = true;
                    break;
                } else if (card == 'N' || card == 'W') {
                    current.hand.erase(current.hand.begin() + i);
                    use_aoe(current_player_idx, card);
                    card_used = true;
                    break;
                } else if (card == 'F') {
                    // 寻找目标
                    current.hand.erase(current.hand.begin() + i);
                    // 简化目标选择
                    int target = -1;
                    for (int j = (current_player_idx + 1) % n; j != current_player_idx; j = (j + 1) % n) {
                        if (!players[j].dead) {
                            target = j;
                            break;
                        }
                    }
                    if (target != -1) {
                        use_duel(current_player_idx, target);
                    }
                    card_used = true;
                    break;
                } else if (card == 'K' && (used_kill == false || current.has_zgln)) {
                    // 寻找目标
                    int target = -1;
                    for (int j = (current_player_idx + 1) % n; j != current_player_idx; j = (j + 1) % n) {
                        if (!players[j].dead) {
                            target = j;
                            break;
                        }
                    }
                    if (target != -1) {
                        used_kill = true;
                        current.hand.erase(current.hand.begin() + i);
                        // 简化闪的判断
                        Player& target_p = players[target];
                        auto it = find(target_p.hand.begin(), target_p.hand.end(), 'D');
                        if (it != target_p.hand.end()) {
                            target_p.hand.erase(it);
                        } else {
                            target_p.hp -= 1;
                            if (target_p.hp <= 0) {
                                handle_death(target, current_player_idx);
                            }
                        }
                        card_used = true;
                        break;
                    }
                }
            }
            if (!card_used || game_over) break;
        }

        // 检查游戏是否结束
        if (game_over) break;

        // 移动到下一个玩家
        current_player_idx = (current_player_idx + 1) % n;
        while (players[current_player_idx].dead) {
            current_player_idx = (current_player_idx + 1) % n;
        }
    }

    // 输出结果
    cout << winner << endl;
    for (const auto& p : players) {
        if (p.dead) {
            cout << "DEAD" << endl;
        } else {
            for (size_t i = 0; i < p.hand.size(); ++i) {
                if (i > 0) cout << " ";
                cout << p.hand[i];
            }
            cout << endl;
        }
    }

    return 0;
}
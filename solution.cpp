#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<list>
#include<assert.h>

// #define DEBUG

#ifdef DEBUG
#undef DEBUG
#define DEBUG true
#else
#define NDEBUG
constexpr bool DEBUG=true;
#endif
#define debug if constexpr(DEBUG)
using ll=long long;
using ull=unsigned long long;
ll n=0;
struct callback{
    virtual void run()=0;
    virtual ~callback(){};
};
struct event{
    virtual void attach(callback*)=0;
    virtual void tigger()=0;
};
class event_once:event{
    private:
    std::queue<callback*> callbacks;
    public:
    virtual void attach(callback*c){
        debug std::clog<<"attach.\n";
        callbacks.push(c);
    }
    virtual void tigger(){
        debug std::clog<<"tigger.\n";
        for(;!callbacks.empty();callbacks.pop()) callbacks.front()->run(),delete callbacks.front();
    }
};
class character;
class team;
class game;
class game_over_exeception:public std::exception{
    private:
    team*t;
    public:
    game_over_exeception():t(NULL){}
    game_over_exeception(team*t_){
        t=t_;
    }
    const team* get_team()const noexcept{
        return t;
    }
};
class skill{
    protected:
    ll x,y,z;
    public:
    skill(ll x_,ll y_,ll z_){
        x=x_;
        y=y_;
        z=z_;
    }
    virtual ll get_skill_id()const=0;
    inline void print_info()const{
        debug std::clog<<"\tskill #"<<get_skill_id()<<" ("<<x<<","<<y<<","<<z<<")\n";
    }
    virtual void run(character*)=0;
    virtual ~skill(){}
};

class character{
    private:

    protected:
    static ll cnt_id;
    ll id;
    ll talent_x,talent_y;
    bool is_die_;
    skill*s;
    std::queue<character*>attack_order;
    public:
    ll hp,mhp,mp,mmp,atk,datk,def,ddef;
    team *self_team,*enemy_team;
    character(){
        id=cnt_id++;
    }
    inline ll A(){return std::max(atk+datk,1ll);}
    inline ll D(){return std::max(def+ddef,0ll);}
    friend class character_init_;
    friend class skill;
    inline ll get_id(){
        return id;
    }
    inline void print_info();
    virtual ll get_talent_id()=0;
    //hp:
    virtual void die();
    virtual ll get_hp(){
        return hp;
    }
    virtual void de_hp(ll dhp){
        debug{
            std::clog<<"character #"<<id<<" de_hp "<<dhp;
            assert(dhp>=0);
        }
        debug std::clog<<" hp "<<hp;
        hp-=dhp;
        debug std::clog<<" => "<<hp<<"\n";
        if(hp<=0){
            die();
        }else{
            add_mp(1ll);
        };
    }
    virtual ll get_de_hp(ll damage,ll real_damage){
        debug{
            std::clog<<"character #"<<id<<" get_de_hp damage="<<damage<<" real_damage="<<real_damage<<" dhp "<<std::max(damage-D(),0ll)+real_damage<<"\n";
            assert(damage>=0);
            assert(real_damage>=0);
        }
        return std::max(damage-D(),0ll)+real_damage;
    }
    virtual void add_hp(ll dhp){
         debug{
            std::clog<<"character #"<<id<<" add_hp "<<dhp;
            assert(dhp>=0);
        }
        debug std::clog<<" hp "<<hp;
        hp=std::min(hp+dhp,mhp);
        debug std::clog<<" => "<<hp<<"\n";
    }
    inline bool is_die(){
        return is_die_;
    }
    //mp
    virtual ll get_mp(){
        return mp;
    }
    inline void de_mp(ll dmp){
        debug{
            std::clog<<"character #"<<id<<"de_mp "<<dmp;
            assert(dmp>=0);
        }
        debug std::clog<<" mp "<<mp;
        mp=std::max(mp-dmp,0ll);
        debug std::clog<<" => "<<mp<<"\n";
    }
    virtual void add_mp(ll dmp){
        debug{
            std::clog<<"character #"<<id<<" add_mp "<<dmp;
            assert(dmp>=0);
        }
        debug std::clog<<" mp "<<mp;
        mp=std::min(mp+dmp,mmp);
        debug std::clog<<" => "<<mp<<"\n";
    }
    //skill
    inline void set_skill(skill*s_){
        if(s) delete s;
        s=s_;
    }
    inline const skill* get_skill(){
        return s;
    }
    virtual bool can_use_skill(){
        return mp==mmp;
    }
    virtual void use_skill(){
        debug{
            std::clog<<"character #"<<id<<" use_skill\n";
            s->print_info();
            assert(can_use_skill());
        }
        mp=0;
        s->run(this);
        add_mp(1ll);
    }

    //attack
    virtual character* get_attack_target(){
        while((!attack_order.empty())&&attack_order.front()->is_die_) attack_order.pop();
        return attack_order.empty()?(NULL):(attack_order.front());
    }
    virtual ll get_attack_de_hp(){
        debug{
            std::clog<<"character #"<<id<<" get_attack_de_hp\n";
        }
        auto target=get_attack_target();
        return target?target->get_de_hp(A(),0ll):0ll;
    }
    virtual ll get_attack_target_hp(){
        auto target=get_attack_target();
        return target?target->hp:0ll;
    }
    virtual void attack(){
        debug{
            std::clog<<"character #"<<id<<" atack";
        }
        auto target=get_attack_target();
        debug std::clog<<" character #"<<((target)?(target->get_id()):(-1))<<"\n";
        if(target)target->de_hp(get_attack_de_hp());
        debug assert(target!=NULL);
        add_mp(1ll);
    }
    //other
    virtual void after_action(){
        debug{
            std::clog<<"character #"<<id<<" after_action\n";
        }
        add_mp(1ll);
    }
};
ll character::cnt_id=0;
class character_init_;
class character_talent_0:public character{
    virtual ll get_talent_id(){
        return 0;
    }
};
class character_talent_1:public character{
    virtual ll get_talent_id(){
        return 1;
    }
    virtual ll get_de_hp(ll damage,ll real_damage){                     //talent#1 该角色免疫一半真实伤害
        debug{
            std::clog<<"character #"<<id<<" get_de_hp(talent#1) damage="<<damage<<" real_damage="<<real_damage<<" dhp "<<std::max(damage-D(),0ll)+real_damage-(real_damage>>1)<<"\n";
            assert(damage>=0);
            assert(real_damage>=0);
        }
        return std::max(damage-D(),0ll)+real_damage-(real_damage>>1);
    }
};
class character_talent_2:public character{
    virtual ll get_talent_id(){
        return 2;
    }
    virtual ll get_attack_de_hp(){                                      //talent#2 该角色每次普通攻击会附加的 x 点真实伤害
        debug{
             std::clog<<"character #"<<id<<" get_attack_de_hp(talent#2)\n";
        }
        auto target=get_attack_target();
        return target?target->get_de_hp(A(),talent_x):0ll;
    }
};
class character_talent_3:public character{
    virtual ll get_talent_id(){
        return 3;
    }
    virtual void after_action(){                                        //talent#3 该角色每次己方行动结束后回复 x 点生命值，并额外回复 y 点能量值
        debug{
            std::clog<<"character #"<<id<<" after_action(talent#3)\n";
        }
        add_hp(talent_x);
        add_mp(1ll+talent_y);
    }
};
class character_talent_4:public character{
    virtual ll get_talent_id(){
        return 4;
    }
    virtual ll get_attack_de_hp(){                                      //talent#4 该角色的普通攻击被视为真实伤害攻击，即该角色的普通攻击不造成伤害，但造成 A 点真实伤害。
        debug{
            std::clog<<"character #"<<id<<" get_attack_de_hp(talent#4)\n";
        }
        auto target=get_attack_target();
        return target?target->get_de_hp(0ll,A()):0ll;
    }
};
class character_talent_5:public character{
    virtual ll get_talent_id(){
        return 5;
    }
    virtual void attack(){                                              //talent#5 该角色每次进行普通攻击后，回复 x 点生命值
        debug{
            std::clog<<"character #"<<id<<" ttack(talent#5) ";
        }
        auto target=get_attack_target();
        debug std::clog<<" character #"<<((target)?(target->get_id()):(-1))<<"\n";
        if(target)target->de_hp(get_attack_de_hp());
        debug assert(target!=NULL);
        add_hp(talent_x);
        add_mp(1ll);
    }
    virtual void use_skill(){                                           //talent#5 该角色每次发动技能后，额外回复 y 点能量值
        debug{
            std::clog<<"character #"<<id<<" use_skill(talent#5)\n";
            s->print_info();
            assert(can_use_skill());
        }
        mp=0;
        s->run(this);
        add_mp(1ll+talent_y);
    }
};
class team{
    private:
    std::list<character*>alive_characters;
    std::vector<character*>all_characters;
    std::string team_name;
    team* enemy_team;
    protected:
    public:
    friend class character_init_;
    ll team_dmp_after_action;
    team(std::string const&name,team*enemy_team_){
        team_name=name;
        enemy_team=enemy_team_;
    }
    inline character*get_character_by_id(ll id)const{
        assert(id<=n);
        if(id<=n){
            return all_characters[id-1];
        }
        return NULL;
    }
    inline std::list<character*> const& get_alive_characters()const{
        return alive_characters;
    }
    inline std::vector<character*> const& get_all_characters()const{
        return all_characters;
    }
    inline std::string const& get_team_name()const{
        return team_name;
    }
    inline void update_alive_characters(){
        for(std::list<character*>::iterator i=alive_characters.begin();i!=alive_characters.end();){
            auto tmp=i;
            i++;
            if((*tmp)->is_die()) alive_characters.erase(tmp);
        }
        if(alive_characters.empty()){
            throw game_over_exeception(enemy_team);
        }
    }
    inline void init();
    inline void action();
    inline void after_action();
};
class game{
    private:
    static constexpr ll MAX_ROUND=23333;
    std::vector<character*>all_characters;
    ll cnt_round;
    event_once run_round_end[MAX_ROUND],run_init_end;
    team team1{"Alice",&team2},team2{"Bob",&team1};
    public:
    game(){
        cnt_round=1;
    }
    inline void attach_run_round_end(ll round,callback*c){
        debug std::clog<<"attach @"<<round<<"\n";
        debug assert(round>=cnt_round);
        if(round<=MAX_ROUND){
            run_round_end[round].attach(c);
        }else{
            delete c;
        }
    }
    inline void attach_tun_init_end(callback*c){
        run_init_end.attach(c);
    }
    inline ll get_cnt_round()const{
        return cnt_round;
    }
    inline std::vector<character*> const& get_all_characters(){
        return all_characters;
    }
    void print_info(){
        // debug{
            std::clog<<"round_cnt "<<cnt_round<<"\n";
            std::clog<<"team "<<team1.get_team_name()<<"\n";
            for(auto i:team1.get_all_characters())i->print_info();
            std::clog<<"team "<<team2.get_team_name()<<"\n";
            for(auto i:team2.get_all_characters())i->print_info();
            std::clog<<"\n\n";
        // }
    }
    inline void init(){
        debug std::clog<<"game init.....\n";
        std::cin>>n;
        team1.init();
        team2.init();
        for(auto const&i:team1.get_all_characters()) all_characters.push_back(i);
        for(auto const&i:team2.get_all_characters()) all_characters.push_back(i);
        run_init_end.tigger();
        debug std::clog<<"game init.\n";
        debug{
            print_info();
        }
    }
    inline void play(){
        debug std::clog<<"round "<<cnt_round<<'\n';
        team1.action();
        team1.after_action();
        team2.action();
        team2.after_action();
        run_round_end[cnt_round].tigger();
        debug std::clog<<"round "<<cnt_round<<" end.\n";
        cnt_round++;
        if(cnt_round==MAX_ROUND){
            throw game_over_exeception();
        }
    }
};
game g;

class skill_0:public skill{
    /*
    skill#0:
    该角色的主动技能毫无作用
    */
    public:
    skill_0(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 0;
    }
    virtual void run(character*self){

    }
};
class skill_1:public skill{
    /*
    skill#1:
    对敌方所有角色造成 x 点伤害，然后使敌方所有角色能量值减少 int(enemy_mp/10) 
    */
    public:
    skill_1(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 1;
    }
    virtual void run(character*self){
        // for(auto i:self->enemy_team->get_all_characters()) i->de_hp(i->get_de_hp(x,0ll));
        for(auto i=self->enemy_team->get_alive_characters().begin(),e=self->enemy_team->get_alive_characters().end(),t=i;i!=e;t=i){//对敌方所有角色造成 x 点伤害
            i++;
            (*t)->de_hp((*t)->get_de_hp(x,0ll));
        }
        for(auto i:self->enemy_team->get_all_characters()) i->de_mp((i->mp)/10ll);//然后使敌方所有角色能量值减少int(enemy_mp/10)
    }
};
class skill_2:public skill{
    /*
    skill#2:
    对敌方所有角色造成 A 点真实伤害。
    */
    public:
    skill_2(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 2;
    }
    virtual void run(character*self){
        // for(auto i:self->enemy_team->get_alive_characters()) i->de_hp(i->get_de_hp(0ll,self->A()));
        for(auto i=self->enemy_team->get_alive_characters().begin(),e=self->enemy_team->get_alive_characters().end(),t=i;i!=e;t=i){//对敌方所有角色造成 A 点真实伤害。
            i++;
            (*t)->de_hp((*t)->get_de_hp(0ll,self->A()));
        }
    }
};
class skill_3:public skill{
    /*
    skill#3:
    对敌方所有目标造成 min(int(enemy_mhp/10),x×A) 点伤害
    */
    public:
    skill_3(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 3;
    }
    virtual void run(character*self){
        // for(auto i:self->enemy_team->get_alive_characters()) i->de_hp(i->get_de_hp(std::min(i->mhp/10,x*self->A()),0ll));
        for(auto i=self->enemy_team->get_alive_characters().begin(),e=self->enemy_team->get_alive_characters().end(),t=i;i!=e;t=i){//对敌方所有目标造成 min(int(enemy_mhp/10),x×A) 点伤害
            i++;
            (*t)->de_hp((*t)->get_de_hp(std::min((*t)->mhp/10,x*self->A()),0ll));
        }
    }
};
class skill_4:public skill{
    /*
    skill#4:
    从发动技能开始，到第 t+x−1 回合结束期间，在己方行动结束时，己方全体角色额外回复 y 点能量值
    */
    private:
    struct skill_4_callback:callback{
        ll *p_team_dmp,y;
        skill_4_callback(team*self_team_,ll y_){
            p_team_dmp=&self_team_->team_dmp_after_action,y=y_;
        }
        void run(){
            *p_team_dmp-=y;
        }
    };
    public:
    skill_4(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 4;
    }
    virtual void run(character*self){
        self->self_team->team_dmp_after_action+=y;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_4_callback(self->self_team,y));
    }
};
class skill_5:public skill{
    /*
    skill#5:
    优先目标的防御力增益减小 x，然后对优先目标造成 A 点真实伤害
    */
    public:
    skill_5(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 5;
    }
    virtual void run(character*self){
        auto target=self->get_attack_target();
        if(target){
            target->ddef-=x;
            target->de_hp(target->get_de_hp(0ll,self->A()));
        }
    }
};
class skill_6:public skill{
    /*
    skill#6:
    对优先目标造成 A 点真实伤害，另外，从发动技能开始，到第 t+x−1 回合结束期间，敌方所有角色攻击力增益减小 y
    */
    private:
    struct skill_6_callback:callback{
        team*enemy_team;
        ll y;
        skill_6_callback(team*enemy_team_,ll y_){
            enemy_team=enemy_team_,y=y_;
        }
        void run(){
            for(auto i:enemy_team->get_alive_characters()) i->datk+=y;
        }
    };
    public:
    skill_6(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 6;
    }
    virtual void run(character*self){
        auto target=self->get_attack_target();
        if(target){
            target->de_hp(target->get_de_hp(0ll,self->A()));
        }
        for(auto i:self->enemy_team->get_alive_characters()) i->datk-=y;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_6_callback(self->enemy_team,y));
    }
};
class skill_7:public skill{
    /*
    skill#7:
    使己方未【死亡】角色生命值最低（如有多个，则编号最小）的恰好一名角色回复 z 点生命值,
    从发动技能开始到第 t+x−1 回合结束期间，己方所有角色攻击力增益增大 y。
    */
    private:
    struct skill_7_callback:callback{
        team*self_team;
        ll y;
        skill_7_callback(team*self_team_,ll y_){
            self_team=self_team_,y=y_;
        }
        void run(){
            for(auto i:self_team->get_alive_characters()) i->datk-=y;
        }
    };
    public:
    skill_7(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 7;
    }
    virtual void run(character*self){
        character*c=NULL;ll min_hp=1e11;
        for(auto i:self->self_team->get_alive_characters())if(i->hp<min_hp) c=i,min_hp=i->hp;
        c->add_hp(z);

        for(auto i:self->self_team->get_alive_characters()) i->datk+=y;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_7_callback(self->self_team,y));
    }
};
class skill_8:public skill{
    /*
    skill#8:
    对敌方所有角色造成 A 点伤害
    从该技能造成伤害后，到 t+x−1 回合结束期间，使敌方所有角色防御力增益减小 y
    */
    private:
    struct skill_8_callback:callback{
        team*enemy_team;
        ll y;
        skill_8_callback(team*enemy_team_,ll y_){
            enemy_team=enemy_team_,y=y_;
        }
        void run(){
            for(auto i:enemy_team->get_alive_characters()) i->ddef+=y;
        }
    };
    public:
    skill_8(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 8;
    }
    virtual void run(character*self){
        // for(auto i:self->enemy_team->get_alive_characters()) i->de_hp(i->get_de_hp(self->A(),0ll));
        for(auto i=self->enemy_team->get_alive_characters().begin(),e=self->enemy_team->get_alive_characters().end(),t=i;i!=e;t=i){
            i++;
            (*t)->de_hp((*t)->get_de_hp(self->A(),0ll));
        }
        for(auto i:self->enemy_team->get_alive_characters()) i->ddef-=y;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_8_callback(self->enemy_team,y));
    }
};
class skill_9:public skill{
    /*
    skill#9:
    己方全体角色回复 z 点生命值
    从发动技能开始，到第 t+x−1 回合结束期间，己方所有角色防御力增益增大 y
    */
    private:
    struct skill_9_callback:callback{
        team*self_team;
        ll y;
        skill_9_callback(team*self_team_,ll y_){
            self_team=self_team_,y=y_;
        }
        void run(){
            for(auto i:self_team->get_alive_characters()) i->ddef-=y;
        }
    };
    public:
    skill_9(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 9;
    }
    virtual void run(character*self){
        for(auto i:self->self_team->get_alive_characters()) i->add_hp(z);
        for(auto i:self->self_team->get_alive_characters()) i->ddef+=y;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_9_callback(self->self_team,y));
    }
};
class skill_10:public skill{
    /*
    skill#10:
    己方所有角色基础攻击力 atk、基础防御力 def 变为原先的两倍 2×atk,2×def
    未【死亡】角色生命值 hp 变为 max(int(mhp/2),hp),能量值 mp 变为 max(int(mmp/2),mp)
    从发动技能开始，到第 t+x−1 回合结束期间，己方行动结束时，己方所有角色额外回复 1 点能量值。
    在第 t+x−1 回合结束时，若敌方仍有角色未【死亡】，则己方全体角色强制生命值清零，并被判定为【死亡】。
    另外，在发动该技能时，场上所有拥有这个技能的角色（包括自己）的技能将被强制替换为 talent#0
    */
    private:
    struct skill_10_callback:callback{
        team*self_team;
        skill_10_callback(team*self_team_){
            self_team=self_team_;
        }
        void run(){
            debug std::clog<<"skill 10 callback.\n";
            self_team->team_dmp_after_action-=1;
            // for(auto i:self_team->get_alive_characters()) i->de_hp(i->hp);
            while(!self_team->get_alive_characters().empty()) self_team->get_alive_characters().front()->de_hp(1e11); //保证在游戏中的任意时刻，所有题面中提及到的参数及表达式的绝对值均不超过 1e9 ==> 1e11>hp
        }
    };
    public:
    skill_10(ll x_,ll y_,ll z_):skill(x_,y_,z_){};
    virtual ll get_skill_id()const{
        return 10;
    }
    virtual void run(character*self){
        for(auto i:self->self_team->get_alive_characters()){
            i->atk<<=1,i->def<<=1;
            i->hp=std::max(i->mhp>>1,i->hp),i->mp=std::max(i->mmp>>1,i->mp);
        }
        self->self_team->team_dmp_after_action+=1;
        g.attach_run_round_end(g.get_cnt_round()+x-1,new skill_10_callback(self->self_team));
        for(auto i:self->self_team->get_alive_characters()) if(i->get_skill()->get_skill_id()==10)i->set_skill(new skill_0(0ll,0ll,0ll));
        for(auto i:self->enemy_team->get_alive_characters()) if(i->get_skill()->get_skill_id()==10)i->set_skill(new skill_0(0ll,0ll,0ll));
        
    }
};

int main(){
    g.init();
    try{
        while(true){
            
            g.play();
            debug{
                g.print_info();
            }
            
        }
    }catch(game_over_exeception&err){
        if(err.get_team()){
            std::cout<<g.get_cnt_round()<<"\n"<<err.get_team()->get_team_name()<<"\n";
            for(auto i:err.get_team()->get_all_characters()) std::cout<<i->get_hp()<<' ';
        }else{
            std::cout<<"...";
        }
    }
    return 0;
}
//::
class character_init_{
    private:
    struct character_init_callback:callback{
        std::queue<ll>*q;
        std::queue<character*>*q_2;
        team*enemy_team;
        character_init_callback(character*c,std::queue<ll>*q_){
            q=q_;
            q_2=&c->attack_order;
            enemy_team=c->enemy_team;
        }
        void run(){
            for(;!q->empty();q->pop()){
                q_2->push(enemy_team->get_character_by_id(q->front()));
                debug assert(enemy_team->get_character_by_id(q->front())!=NULL);
            }
            delete q;
        }
    };
    public:
    inline character*operator()(team*t){
        character*result;
        ll talent,talent_x,talent_y,hp,mp,atk,def,skill,skill_x,skill_y,skill_z;
        std::cin>>hp>>mp>>atk>>def;//四个非负整数 HP,MP,atk,def 分别表示该角色的最大生命值、最大能量值、基础攻击力、基础防御力
        std::queue<ll>*q=new std::queue<ll>;
        for(int i=0,p=0;i<n;i++){//n 个正整数 表示该角色攻击顺序，保证这是一个关于 1 到 n 的排列
            std::cin>>p;
            q->push(p);
        }
        std::cin>>talent>>talent_x>>talent_y;//三个非负整数 tf,x,y 表示天赋编号以及附加参数。如果该天赋附加参数不足两个，那么你可以忽略多余的部分
        std::cin>>skill>>skill_x>>skill_y>>skill_z;//四个非负整数 jn,x,y,z 表示技能编号及附加参数。如果该技能附加参数不足三个，那么你可以忽略多余的部分
        switch (talent){
            case 0:result=new character_talent_0();break;
            case 1:result=new character_talent_1();break;
            case 2:result=new character_talent_2();break;
            case 3:result=new character_talent_3();break;
            case 4:result=new character_talent_4();break;
            case 5:result=new character_talent_5();break;
        }
        result->hp=result->mhp=hp,result->mmp=mp,result->atk=atk,result->def=def,result->mp=result->ddef=result->datk=0ll;
        result->talent_x=talent_x,result->talent_y=talent_y;
        result->self_team=t,result->enemy_team=t->enemy_team;
        g.attach_tun_init_end(new character_init_callback(result,q));
        switch (skill){
            case 0:result->set_skill(new skill_0(skill_x,skill_y,skill_z));break;
            case 1:result->set_skill(new skill_1(skill_x,skill_y,skill_z));break;
            case 2:result->set_skill(new skill_2(skill_x,skill_y,skill_z));break;
            case 3:result->set_skill(new skill_3(skill_x,skill_y,skill_z));break;
            case 4:result->set_skill(new skill_4(skill_x,skill_y,skill_z));break;
            case 5:result->set_skill(new skill_5(skill_x,skill_y,skill_z));break;
            case 6:result->set_skill(new skill_6(skill_x,skill_y,skill_z));break;
            case 7:result->set_skill(new skill_7(skill_x,skill_y,skill_z));break;
            case 8:result->set_skill(new skill_8(skill_x,skill_y,skill_z));break;
            case 9:result->set_skill(new skill_9(skill_x,skill_y,skill_z));break;
            case 10:result->set_skill(new skill_10(skill_x,skill_y,skill_z));break;
        }
        return result;
    }
}character_init;
//



//character::
inline void character::print_info(){
    debug{
        std::clog<<"character #"<<id<<" @team"<<self_team->get_team_name()<<"\n"
                 <<"\t"<<"hp(<"<<mhp<<")"<<hp<<"  mp(<"<<mmp<<")"<<mp<<"  atk "<<atk<<"(+"<<datk<<")  def "<<def<<"(+"<<ddef<<")\n"
                 <<"\t"<<"talent #"<<get_talent_id()<<" ("<<talent_x<<","<<talent_y<<")\n";
                   s->print_info();
    }
}
inline void character::die(){
    debug std::clog<<"character #"<<id<<" die.\n";
    is_die_=true;
    hp=0;
    self_team->update_alive_characters();
}
//character

//team::
inline void team::init(){
    debug std::clog<<"team "<<team_name<<" init...\n";
    character*tmp;
    for(int i=0;i<n;i++){
        tmp=character_init(this);
        all_characters.push_back(tmp);
        alive_characters.push_back(tmp);
    }
    debug std::clog<<"team "<<team_name<<" init.\n";
}
inline void team::action(){
    debug std::clog<<"team "<<team_name<<" action...\n";
    character*c=NULL;ll max_skill_id=-1;
    debug std::clog<<"\tuse skill?\n";
    for(auto i:alive_characters){
        if((i->can_use_skill())&&(i->get_skill()->get_skill_id()>=max_skill_id)){
            c=i;
            max_skill_id=i->get_skill()->get_skill_id();
        }
    }
    if(c){
        debug std::clog<<"\tuse skill.\n";
        c->use_skill();
    }else{
        ll attack_target_hp=0,attack_de_hp=0;
        for(auto i:alive_characters){
            if((i->get_attack_target_hp()>attack_target_hp)||((i->get_attack_target_hp()==attack_target_hp)&&(i->get_attack_de_hp()>=attack_de_hp))){
                c=i;
                attack_de_hp=i->get_attack_de_hp();
                attack_target_hp=i->get_attack_target_hp();
            }
        }
        debug std::clog<<"\tattack.\n";
        c->attack();
    }
    debug std::clog<<"team "<<team_name<<" action.\n";
}
inline void team::after_action(){
    debug std::clog<<"team "<<team_name<<" after_action...\n";
    for(auto i:alive_characters) i->after_action(),i->add_mp(team_dmp_after_action);
    debug std::clog<<"team "<<team_name<<" after_action.\n";
}
//team
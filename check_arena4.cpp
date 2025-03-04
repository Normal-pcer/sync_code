#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <ranges>
#include <stack>
#include <string>
#include <vector>
const int height=65,width_of_char=38,num_of_chars=16;
const char char_list[num_of_chars+1]={'0','1','2','3','4','5','6','7','8','9','(',')','+','-','*','/','?'};
class ARG{
public:
    float x;
    ARG()=default;
    explicit ARG(float x_):x(x_){}
};
template<typename T>
concept IS_ARG=requires(T& t){t.x;};
template<typename T>
concept WITH_GRAD=requires(T& t){t.x_grad;};
template<typename T>
concept WITH_BP=requires(T& t,float l){t.learn(l);};
template<typename T1,typename T2>
concept CAN_TRAIN=IS_ARG<T1>&&WITH_GRAD<T1>&&IS_ARG<T2>&&WITH_BP<T2>;
float from_s(std::string_view s){
    int y=0;
    for(int i=2;i>=0;i--){
        y*=92;
        y+=s[i]-33-(s[i]>'"')-(s[i]>'\\');
    }
    bool f=y%2;
    y/=2;
    float x;
    if(y<16928){
        x=(static_cast<float>(y)+0.5)/16928;
    }else{
        x=std::ldexp(1+(static_cast<float>(y%16928)+0.5)/16928,y/16928-1);
    }
    return std::ldexp((f?-x:x),-18);
}
template<IS_ARG T>
void from_s(T* a,std::string_view s){
    for(int i=0;i<s.size();i+=3){
        a[i/3].x=from_s(s.substr(i,3));
    }
}
template<IS_ARG T1,IS_ARG T2,int depth,int input_height,int conv_num,int conv_size,int pad,int pool_size>
class CNN{
    static const int conv_res_height=input_height+2*pad+1-conv_size;
    static const int pool_res_height=conv_res_height/pool_size;
    static float activation_func(float x){
        return (x>0)?x:0;
    }
    static float activation_func_deriv(float x){
        return (x>0)?1:0;
    }
    T2 weight[conv_num][depth][conv_size][conv_size];
    T2 bias[conv_num];
    std::vector<std::array<T1,conv_res_height>> convolution_res[conv_num];
    std::vector<std::array<T1,input_height+2*pad>> input[depth];
    void padding(const auto& in){
        for(int c=0;c<depth;c++){
            input[c].resize(in[c].size()+2*pad);
            for(int i=0;i<pad;i++){
                input[c][i].fill(T1(0));
                input[c][in[c].size()+i+pad].fill(T1(0));
            }
            for(int i=0;i<in[c].size();i++){
                for(int j=0;j<pad;j++){
                    input[c][i+pad][j].x=0;
                    input[c][i+pad][j+input_height+pad].x=0;
                }
                for(int j=0;j<input_height;j++){
                    input[c][i+pad][j+pad]=in[c][i][j];
                }
            }
        }
    }
    void convolution(const auto& in){
        padding(in);
        int conv_res_width=input[0].size()+1-conv_size;
        for(int i=0;i<conv_num;i++){
            convolution_res[i].resize(conv_res_width);
            for(int j=0;j<conv_res_width;j++){
                for(int k=0;k<conv_res_height;k++){
                    convolution_res[i][j][k].x=bias[i].x;
                }
            }
            for(int c=0;c<depth;c++){
                std::array<std::array<float,conv_size>,conv_size> wgt;
                for(int x=0;x<conv_size;x++){
                    for(int y=0;y<conv_size;y++){
                        wgt[x][y]=weight[i][c][x][y].x;
                    }
                }
                for(int j=0;j<conv_res_width;j++){
                    for(int k=0;k<conv_res_height;k++){
                        float cvr=0;
                        for(int x=0;x<conv_size;x++){
                            for(int y=0;y<conv_size;y++){
                                cvr+=input[c][j+x][k+y].x*wgt[x][y];
                            }
                        }
                        convolution_res[i][j][k].x+=cvr;
                    }
                }
            }
        }
    }
    void activate(){
        for(int i=0;i<conv_num;i++){
            int conv_res_width=convolution_res[i].size();
            for(int j=0;j<conv_res_width;j++){
                for(int k=0;k<conv_res_height;k++){
                    convolution_res[i][j][k].x=activation_func(convolution_res[i][j][k].x);
                }
            }
        }
    }
    void pool(){
        for(int i=0;i<conv_num;i++){
            const int conv_res_width=convolution_res[i].size();
            const int filled_width=(conv_res_width+pool_size-1)/pool_size;
            convolution_res[i].resize(filled_width*pool_size);
            result[i].resize(filled_width);
            for(int j=conv_res_width;j<filled_width*pool_size;j++){
                convolution_res[i][j].fill(T1(0));
            }
            for(int j=0;j<filled_width;j++){
                result[i][j].fill(T1(0));
                for(int x=0;x<pool_size;x++){
                    for(int k=0;k<pool_res_height;k++){
                        for(int y=0;y<pool_size;y++){
                            if(convolution_res[i][j*pool_size+x][k*pool_size+y].x>result[i][j][k].x){
                                result[i][j][k].x=convolution_res[i][j*pool_size+x][k*pool_size+y].x;
                            }
                        }
                    }
                }
            }
        }
    }
public:
    std::array<std::vector<std::array<T1,pool_res_height>>,conv_num> result;
    CNN(std::string_view s){
        from_s(&weight[0][0][0][0],s.substr(0,3*conv_num*depth*conv_size*conv_size));
        from_s(&bias[0],s.substr(3*conv_num*depth*conv_size*conv_size,3*conv_num));
    }
    void forward_propagation(const auto& in){
        convolution(in);
        activate();
        pool();
    }
};
template<IS_ARG T1,IS_ARG T2,int depth,int input_size,int state_size,int output_size>
class RNN{
    static float activation_func(float x){
        return std::tanh(x);
    }
    static float activation_func_deriv(float x){
        return 1-x*x;
    }
    T2 input_weight[2][state_size][depth][input_size];
    T2 state_weight[2][state_size][state_size];
    T2 state_bias[2][state_size];
    T2 full_connect_weight[2][state_size][output_size];
    std::vector<std::array<T1,state_size>> state[2];
    void rnn_one_layer(int flag,int i,std::array<std::vector<std::array<T1,input_size>>,depth>& input){
        int b=1-2*flag;
        state[flag][i+b].fill(T1(0));
        for(int j=0;j<state_size;j++){
            for(int c=0;c<depth;c++){
                for(int k=0;k<input_size;k++){
                    state[flag][i+b][j].x+=input_weight[flag][j][c][k].x*input[c][i-flag][k].x;
                }
            }
            for(int k=0;k<state_size;k++){
                state[flag][i+b][j].x+=state_weight[flag][j][k].x*state[flag][i][k].x;
            }
            state[flag][i+b][j].x+=state_bias[flag][j].x;
            state[flag][i+b][j].x=activation_func(state[flag][i+b][j].x);
            for(int k=0;k<output_size;k++){
                result[i-flag][k].x+=state[flag][i+b][j].x*full_connect_weight[flag][j][k].x;
            }
        }
    }
public:
    std::vector<std::array<T1,output_size>> result;
    RNN(std::string_view s){
        from_s(&input_weight[0][0][0][0],s.substr(0,6*state_size*depth*input_size));
        from_s(&state_weight[0][0][0],s.substr(6*state_size*depth*input_size,6*state_size*state_size));
        from_s(&state_bias[0][0],s.substr(6*state_size*(depth*input_size+state_size),6*state_size));
        from_s(&full_connect_weight[0][0][0],s.substr(6*state_size*(depth*input_size+state_size+1),6*state_size*output_size));
    }
    void forward_propagation(std::array<std::vector<std::array<T1,input_size>>,depth>& input){
        result.resize(input[0].size());
        for(auto& i:result){
            i.fill(T1(0));
        }
        state[0].resize(input[0].size()+1);
        state[0][0].fill(T1(0));
        for(int i=0;i<input[0].size();i++){
            rnn_one_layer(0,i,input);
        }
        state[1].resize(input[0].size()+1);
        state[1][input[0].size()].fill(T1(0));
        for(int i=input[0].size();i>0;i--){
            rnn_one_layer(1,i,input);
        }
    }
};
template<IS_ARG T1,IS_ARG T2,int input_size>
class CTC{
    std::vector<std::vector<float>> alpha_hat;
    std::vector<std::vector<float>> beta_hat;
    std::vector<int> l;
    std::vector<std::array<float,input_size>> log_of_res;
    float sum,max_x;
    int L,T;
    static float log_sum_exp(float a,float b){
        if(std::isinf(a)&&std::isinf(b)){
            return -HUGE_VALF;
        }
        return std::max(a,b)+std::log(1+std::exp(-std::abs(a-b)));
    }
    void softmax(std::vector<std::array<T1,input_size>>& input){
        log_of_res.resize(input.size());
        for(int i=0;i<input.size();i++){
            sum=0;
            max_x=input[i][0].x;
            for(T1& j:input[i]){
                if(j.x>max_x){
                    max_x=j.x;
                }
            }
            for(int j=0;j<=num_of_chars;j++){
                log_of_res[i][j]=input[i][j].x-max_x;
                input[i][j].x=std::exp(log_of_res[i][j]);
                sum+=input[i][j].x;
            }
            for(int j=0;j<=num_of_chars;j++){
                log_of_res[i][j]-=std::log(sum);
                input[i][j].x/=sum;
            }
        }
    }
public:
    std::vector<int> result;
    void forward_propagation(std::vector<std::array<T1,input_size>>& input){
        int l=input_size;
        result.clear();
        for(int i=0;i<input.size();i++){
            float max_p=-1;
            int opt_j=input_size;
            for(int j=0;j<input_size;j++){
                if(input[i][j].x>max_p){
                    opt_j=j;
                    max_p=input[i][j].x;
                }
            }
            if(opt_j!=input_size-1&&opt_j!=l){
                result.push_back(opt_j);
            }
            l=opt_j;
        }
        softmax(input);
    }
};
template<IS_ARG T1,IS_ARG T2>
class CRNN_CTC_MODEL{
public:
    CNN<T1,T2,1,65,8,4,1,2> cnn1;
    CNN<T1,T2,8,32,8,3,1,2> cnn2;
    CNN<T1,T2,8,16,4,3,1,2> cnn3;
    RNN<T1,T2,4,8,24,num_of_chars+1> rnn;
    CTC<T1,T2,num_of_chars+1> ctc;
    void forward_propagation(auto& input){
        cnn1.forward_propagation(input);
        cnn2.forward_propagation(cnn1.result);
        cnn3.forward_propagation(cnn2.result);
        rnn.forward_propagation(cnn3.result);
        ctc.forward_propagation(rnn.result);
    }
    CRNN_CTC_MODEL(std::string cnn1_str,std::string cnn2_str,std::string cnn3_str,std::string rnn_str):cnn1(cnn1_str),cnn2(cnn2_str),cnn3(cnn3_str),rnn(rnn_str){}
};
int prec(char op){
    switch(op){
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
    }
    return 0;
}
int apply_op(int a,int b,char op){
    switch(op){
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            return a/b;
    }
    return 0;
}
int eval(std::string exp){
    std::stack<int> numbers;
    std::stack<char> operators;
    for(int i=0;i<exp.length();i++){
        if(std::isdigit(exp[i])){
            int val=0;
            while(i<exp.length()&&std::isdigit(exp[i])){
                val=val*10+exp[i]-'0';
                i++;
            }
            i--;
            numbers.push(val);
        }else if(exp[i]=='('){
            operators.push(exp[i]);
        }else if(exp[i]==')'){
            while(!operators.empty() && operators.top() !='('){
                int val2=numbers.top();
                numbers.pop();
                int val1=numbers.top();
                numbers.pop();
                char op=operators.top();
                operators.pop();
                numbers.push(apply_op(val1,val2,op));
            }
            if(!operators.empty())
                operators.pop();
        }else{
            while(!operators.empty()&&prec(operators.top())>=prec(exp[i])){
                int val2=numbers.top();
                numbers.pop();
                int val1=numbers.top();
                numbers.pop();
                char op=operators.top();
                operators.pop();
                numbers.push(apply_op(val1, val2, op));
            }
            operators.push(exp[i]);
        }
    }
    while(!operators.empty()){
        int val2=numbers.top();
        numbers.pop();
        int val1=numbers.top();
        numbers.pop();
        char op=operators.top();
        operators.pop();
        numbers.push(apply_op(val1, val2, op));
    }
    return numbers.top();
}
const std::string cnn1_arg="_]daWds(coZe16_Kr]=-`$Fch4lp%h3~]k.aP*alKml_`,RBN!YL$K.gRWOUJ/WmgWova~,[j6cD8_@Hb--btHm5ikVD_E%k]od<#beC`zLcJ``(8YYTYnPck~Thql?kmwj[^elB&e:8__$Rj%gf7cK(F$Mef=d?Dgg5c&&fUofMpV-]__@gf+b0fV.m[aEflmh2Ud&|_FSdQ.lWenN#[T~eD$c-r[#1b6ce9ngIxbh)g#KbDBeZ#Y%-`YbiT>e6#_Nk[;egR!eP@cE?clUcV'ep*UpB^GEYisTcRd_`X0@c{__o3Vs3c&l`#PZPC`0Mdw/_1CNIpWx_V'*RtPe=8b)2m`fbhha4:S,hWGbnGZc)tW<7`+~YbF`~&dtp`hVnj:i:=g~%c^(g9QeWDh7wji!j";
const std::string cnn2_arg="}tf&Fh1vi%Fe8idP:Xdxjj|b*tfU|i<:`+HkpEkQmfbgY=;e_Oh*bY~JZe)iY!h?~e(%]}i`S9eT!hm4d;(b{zgwLb'%j5$f{!dh=n]scRthI]_4=h|yo/opqwdGznxwnz9gRXieBk~0fGxjRGfUa`IFee)i,~e-4gjFgb|XBSj?9[5(e:Yfo3_[Rc<;d|FiQ*gU7l+wq~Oqx}pbpo;*k#,jS]g5OfLsah88`&P)8erj_duYQ2d9Lg*dZcwg7!e}3Z9=T?Wgm.d51c;/pBR`JXebPfLOa$md;Ris5hkKKTNgWTUo5kxSh5ZhX^g&ddmNhRwkmIhG<d`QdiFgy'f%_fb:V#QkQ5c`ni~UcS:]~wWa*cx'_sBjFr[BU]a.keGi/-`w%fd;hl2oTlcO)kbfiE;oO/n[Th'bhA>faldC.k#I`I[Y!Ee'Tp^to1@e/,lM;kIJm1YkkxWfGcjbfk*frqaG-gR]hE%lEPl-~h7l`;QmJgehPo^klq4gtHg1Ej|Ia]0gt$fUdk.pd{l^bViPAchtch7l,?juRb*_YnReYvks`k7*j'ig}VjF7n&b`c^l.#]t6adr_Mfh{,_h5X<wg]JcjAn.)RCk_p-l}Uh(q[~2e-?mZz`h@bOanJ[ba{_W^qaKV}le|E_y7f.IdR]f}Me@~c*2_(p^mPfXF`r(WM7iJ'a!_R5[jDZj,~`=df8&dOdlGNhk!f<?b-pj:?gcZ``|hSlq1El'>Stklj+hE*h*T[}?j<ag|(pQ6g!9SCcci2ikij(=_uKaoNZr*hmwcn>iN8`W_hzghM$c1@X`skN<g{C`g#f{fT0ae{UfOQff.J$6jU/uQXsl#qG}l?,_)>`fndx2cVRccWj86h{Wahtfm+`8Ua/2cj``wab!,eDbeB{Yfp`HYg[1d~=a|,iM-j4oeT%W)<dNxaBkaVi_:gY<2F18fj(e!!Y4RJ3L`($^#YBL-?caeX`eoWc`Q_BDg55RsL_.]didW9oe}C[@Ig#YfRCgTTjbH`OCgDPe*UhR&`JQ]oSjmml3el%bePDip$izhhyjff=inJhzyo4fd)'_t!i>'UAFc[YgzY`$&`o/ij%nwKe*Ii3_h>.c,|gD$QO!_!]Yhva/2Q$+_PRds6^rC`-e`P}WbT[N%cTV]6uP*rf6jh_qc?!g~XeiaiXTcD!`F?j'>gG6^NckDcC?_aLua*Ma6(YH.ckibMASa7YIfkPqkuZaWNog(i]ee^$pR+`:ai)!nS=ZPCWQGp5mgmfe|prU(f9Qcvr_2VZTPeYfe%OfwhXY)fzqd+cY`mo|i]j<dncb6!MkHjI5j#keRohVGc_3`N9_U#X_n[PMTY.i+8o6be&`fA(hxWc!{goMcLWdi1iT!c,2bfUWhRkv1gU2Y-^jLjY{5iT6f_g_6=g8Chk{anyghDdH)i1+gW{jcyT?PkYdiv<h)'i-!K8,_(b^e[[cOm=bq''ppnn;)UpGcb#kT4k{0lp}]FNc84`BSZQ#aq@ZGS_IhdrIiVSf.Eeh'_,.dyedx1cl]g@|Q~Ca)SeH#cbXfM_d2t^I~gTiQN%f2&d(2g_1j^Ua-:kXbcl'_f>U[~gpAf&Jgkziw7c/qh_jhrZZwC_:;drr``xVfcZaj[CeguEe;sghBikBgKF]nCf8-ZFDcJ<h<rZ'IgFDcZ+`Gf`8sc?Ki!BchOY94lzcWnDa7WhB?bIFcdD?l7kQOfH{aA|pINlTR_n}gUJcT?j6Mhk9Zw*cDWdqLY~4c*9j";
const std::string cnn3_arg="-XkvZc)&`K?kn+j<[_-[g/FkQge[bg*`_h/hlRh@dcZ^['+ceP_!*e]Oi4Xi5'bXzh}og]kiJ1g,^gV[iHVg?CSxGiDYeqG_dIcIhRZ.e&,g<Yg''ll|cP^dUTV~og_afDvc'}eO8h9Bca7^f6T%&[Wvc$Bc[iLt9b3eg7Ia:0SY}iY<f}gRAeovtf]_^L;gQtaUdkD)`>DWQMf!}ecTe&S`$XeB:gaQSHIcX*Y8]eZ(`nJ]DxekOi:Yfi!QqUg:Fd4MZ,#gggco1cQ&_<Ucuba&}d*md5(_Rlco,ddtcbkO>j`>)hPl`(Tc!+aLyZTq`6_eBBdTCd*^j$R]kWd](f@PaZ'[cwfM3^l,h[Kbp0e'~h[gbY2]z.f%l`x1hn,hIWVYDi5/g~%dUXgW/in^h8k`uPfg(g0[aRvf_]f(E^*(hJ&bV*gg=ds3cR`dgp_`Kf-Ac=cm^ul{'j}Fhx]eqnf1fc|Dhz<`pNkt$a!XX|*XCPk,2c+Zi#Zgwwc`Bd@`cKfaHLfDUgG?c?b^yw`9Bitpgl?b/Vhn6fh#[Sxcb0_+?Q~{bMFm|`l;fdd/dTd]X^an#f~QcAl[*/fy%Y=+YbVkh,eIKpY!gxii9;m+]V!oe.6_w%[[Ikm=_SNjd)iADb~'knkkUQbYan,4evkb)*khphjSgffkxFi*3fxKbmaiL<e%&dBKg>jk/uXH7bmjd2M]GThxW`9#g/N_:*[Soga1if-d+LnE`g'Ti=doOLijPlnhkz5emIb%MdRWZ$Lg>PX,|e0(aMsZ4%kW!g?~bN5d[$`rAekvb_9[d6b(oc&{d].g90gNVhuU]c-fr<bShiXzgXuhQ9`R`h()f1ycq~hM*_HLfV~cRRaM*cIPZ9D_";
const std::string rnn_arg="yFbC+bnBh0=d0pd&<gN)h|wcBbj[wcGklWJ_YVZZwl}>mugk5JZ.liBycNWhRjiP~Z;Qe3Jc6,hJ}[k*hN+M.Ni/hY%?jZdq@Givzf-ifM^g|IiG|gv.h6S`u__AGgY$nv`i%uk[^l-]mYNo*tkFUhcbco&faAe-eTcQ`&+kd3f^Fb1ceBN]Wedx!]f7ckehH2geGju<i:u^FTjc*jFxkcV_9Whboi-+kfPl~$mY1nd[ji7_O`dUAeCOg#Ae|7i[+]vpd7:YaDC{<cXuim}l,#mD?i0`fGrj#Xgkvh/lg9OnONb]~fj9fWslTYRDjhKee?UlT~kxtchbeG>opok|Pe3$cwwf^{beG[u!e`xgTDlJ+lmuk:je-8b]r[Y=e[RtA{nE<l2}d]^dO.bW&[mZfk(W3!o`Fk()cZl_f1[yhgNvgPjhz<nGEm#kl${Q.|g{qi[GZL+O7<h6oco3gqpdM5dfWiS9aU<H0~c6G[v*U73[H$`rFhb8ih(n:'lD6d|heDKY6Dbz:d=:cvgev(h7]gjab/geb(g6ud./eONdxZg-!hZ,gV?piRt_Oe^xk^|lBq[,hmcao]no8|m>&ki-i1%h.sbZ0^W$dxuixahQ^iJaR1:Q8`W~#c4=`Uh`kwc&!ji]lR[lV+oB*mmakT~j2X`,;g{]i,_c@Xg;?er!ag$Qr<X:!LNHcnKcbuc7rRP3Y`xcfYeWZeb[gX)asrZk/`RuiinhPFc2md.%eRfe3df[;i?~iO1in4dq&`]IgeJeV-eYldl@gMSgd.fM,Huv`.f`q|cPreZ[d<qf03heMfvI[nyi+*b_-cT>a#GdtyeAce;{bv)eknWfad'Bez_hp>jh3k=qk-=h{xZ-[ef.b23LHUecKd_tc]}ab:b.7cWkaUbf=gb9}W~B`-I_gI_0(gi8buR^.sc:|dLKbvlcmP`^TMD,`s]ZS+ddwh1?i1BibCcM?k%:_b>i'!idgi'*n`{o5`Y7?aU=i|_hv5`*Z`r<R~W`T%kqBcm(]!9Z,lb}?NsS>lyfu[fW5W~&idbT^ga{M^<_VU;gf(db>b*KRRcVT%bF5g4afe'[8ld)p`^N^t|ZB+_)d`pQ_Uxcz|hdogQm_MMeC(iSGc;DeAOeatki&oDLnX$o]cls>cYNWGe]eEdmo_ph`vRgS>aXxR8/W<ab@HfeJjc,m*ikPtkNwj*0jM8d'[j71eT(]edfaRc^,ZqAHWXctJY?&d0Dgy4g.'g]Kd!saFyR{ldd$e8WCc4b;,b[gi&Q[R4hc.c%hm-@h$k]hFh%Vj]VYVbfo>b]0lH&kZKbl>a,?]sZf5}dVGe!bl}HnXDmF/knuaMYgyrj_hi>OaV5a#o^pIjK4l?Dm3(ml-gl$h1id/nc%@^I.dY>VCSfg6dEU_vCh$7XE=]LWNeVbXeZFNaN$aJ][?)Y;Nc(~[wybe-iBE`(bi80cx@ZybdH|bf3]~}fX>^>T]z2a.F`:-_Glcd2b#&]/|f`>c,g`2.hB6h{Cgi<gJrcbugz*j)9lY<k])b_8f?P]dhhtVnh#jjsh`hd5$db9WmY]m:ZM*kxPeEsaOWe+kf-I`nBe+&gQ%g$sei:kwT`|iib9jKJjWaev/]!:b,Ni*ElB-hy~g:qgDL_twU;&`G2cf{_hT^#G_[o`hza}8ew=gu,fPJfe%fwkdi'fV'gGZo+&hEwkE>kSWj'UhzWh5g_y]dJ1h:wg$UeJUeL!gk>gHHe^;dC9_}zh}9ie`lDZl@so24kG?g+}gf4j&1d0Af09d0;^wNo~Td)]d{CjN}j4@jH-],5ocEb7_dDW[zNf~uk+:kd;dB_dIqY{Tj+jfU&l9Al5Nm~#p9vod]k5Afw>^~kdpzdbnc)hM}Xe]8c}kgs4Xwm_@WekcgNzb(}_`&evAcD^]NRW(|gi#g@Igu1_n,]8bjexgF/V^}a#Adv-`MA`q*`^3c=@g~XeUUd`6fr)dpocXs`FDldVi);iQ3jlIcr)bBV[(BaCfdN#bi`fYoeA)cUSd#9bhCd#+q'!lkMgCqX?<kp6b:se:mdK;l]}bFIk1{k7mm}Ai4Ja?;m9(kqvcM]e0a^_CeQ%_3zg0zcdCc$`gaseo'kq5_gbaw?h|AfHPkWy`Q+hMMiS6ln`mS>kfLcpMcS%^Oo`kvI{|MqI``tif_lcgiIwh!d`|]VT4Z0%S1NQmkcI`_UN_;$inlk15ofco[`o2]bEXbjBfx<d%3gz^fGKgQMh-ieg%`bfW{|b$:h:5iqtk@Lhj%TW(MMS]Z-_>4Hj%eK8h)4gSpdg0d9Fd^rg%:iWbib<lSVldQgu1cP]_f+m^+kV9j*-oVeR>.XMpgf,m]6j:FmBw_O/kz=jS>giv]NZks(j%8hsndZ3kJVcoxdPularnR<_C)e<3lh{hf_n^ze-Zl==d+nJ.>ZT3hJug:ximYh@hhm:f(@hoQ_$mMX1cLkd94K&Vc{WkHuZ-Dh+}d1h_3AfWiZNwkDmm*um}@lishgYgydh}8P{'deAcZmaDW^AJi'8m5@keGcKYq#7sExsC|s~tl<3g('dR3kz(crIjG&]?=X*3hE#ei.`Aga~IoC4pzto`<pcBex-k'hk42eT)g)ZVGqj9}k-Ei?Gjx+Wr(`pJkM2hG2aVD^6gifI[zH`W>it![I2atmi'xg?Jlhck9phUTe=6mhHk+&krRdxG[Ymf7skdghWls|cm9Hm(EhT/lL0bE8hUopZ!l!>ki}iv?e-6V_2i(xf;onXoZY+]*Ji<_jfch&B[ehc4Zfmajf&nYns?vqinf$DoH0k3!s6E^DZ^c3a)9^4'de<gwZe`~alplf$md:m`}lAzm+Gm['m3<j+Tg|$_AhcB;b.=ZfUb)']YX]sld!KkJKk_Qdy'g0deF'diJe]t]kkZ;9d7%hl6ddpglGd5+f%od,?_.<gst^{Ae&>cP*W>lb?Hf5Ke4~f$4bSaUzO^u8`LV]Qn]Gee>UfHqgB:H.yd(ogAkc4kj9Bkdok5!k=Mijyc=,b1+]qYa`n_E8cVR`zpd^Fdk-_t!hJldH5gwYhHYik$fw|c^8hKqf'3fK%njRlbpm+qh@s[zYMr]d0*m)Ihn]hAsgr?]a#kwViV;msFcFjc!4btC_/(a/Odp=kE`p+9XIHg>>`^rcURjtEhi,gaycpynOSpI^r+Vq[0u#_qr>rzek{ogy)lHkioQcrr`E=`wBcqH`U&d*s].}fm9im3ge9i6&gw'hfyb'1k-cd]fe}aiB8_*U_o_dUIkfCm/ZnzUmfjd'1c9qa6)e'[j3^bLVZohcKk[#M[R}GH9dByj<9gm-eP!hkKhPZjw'iC+]p3`:riy'gB>cV?f~&cG}Zwx_D0l{tiD=^<|b^S[}E]&wekYc{'h9=V=PnrfaILXoOLWZ`tUg?%tGEp#Sh(zjBLm-FV)Te4:Z&%jKAl?tghniYYe'vkQnb}/_z+q6omvj^~lfY2[^_id%jz][l,ipPhK>c%Yghfipqb;KglX]wWg6ek<+k(fk:9kTZk$Eg&!k<Ce$D_SggxchA`kpWltDh|1ikEe&9^%IecTgP>gT}ju(kL?fIMm7*ih|m5Hj;#oxtlE,nhvklHhQZql0tE6t'^xhJwbut[MrP%`lGf{kf%KgwHS?9dCHdXug;qe='mBVibfotllC`pU+qz2srNcckhwUh2Gh6DhNNk85j6~g?^[fZh?2g|)i56jr%il*a4Odm'W@EjBOf<u`4gMAjV&gWbA_gqkX2p$fpV5qLdp6=o(QlJ%_7Qt1NszfrSgr~drK's)?s-Ds?biYWp^lqYYpW0qA5g_Hj:alk#c9%]izYXI^(tgLM`,dYQ$J-$o/Ds#)tM!tIMs?:mNw]Or_h!s;xoQ2o%ZofOn-hmK7oz.n+vl%Prt#szbp`sj*Rks&n2)k*0gnn^cq_x$i4}lc9k6liZlgt3jdqsnCs;&r#pf4Mo}xoB<k'_kJ/c82``@`vfe}jbiDg(=aeOj@]h]=h4MjkOi@~g0qc>=gM4bV0g$&h4a]DjaDMaSm`0T[NDhihig/hiAf<Ef2pTmUa'e`DUl)[Bi9n>,iv3dE7[r[U2#Q#!fnNih=mY9hF'k.Ro:pw6rs;.iwenedkN8m8)i?KTw(_*!faek$Ygh<c=)iUhg^$mTOveZkBVpD3l$*l!Jj_(ahTcSQe}=kWqlzFmrUmn_nnGnY?q5;o-{pFIW/_d{Lct1ef:hvEh.:`dM`jZnvjjH=n,mnF9oVslVJdh;ph0kY+_W[f1:cq_b&fdm@h8&k:;o0%pwuoVNn:gejFg+Iai,igthyGjDCeQeacWg8Nj.Kkk6g:#j>[jJ[[aGcq.g}{m*flw(gpKkHcK3m[.eQp@XXqcqpc%Uc!xg-&W{Wc{&c@gd2S`8SeJ_cvMdH?j4t]?*glRZva[<0`9(e,0e6rgQ%['|d?GWMdd-Dg:zZ@S_`Yi#BcmMe(nm_aq8.p6boM?k~;iY=eb@ft:hY?hn4^Q}dNbZg$iB&kg?[vAhdQjYAkWcknJ]mZe6+ow7r!zp(@o&!p8+p@4Z{qf6xo4Bq6SoINdfkhWE`m(s>-lYNc'{k3DinViMSh^-m%ff0Ck#Sh)niYbdcGVk|eE&aPFlEyoY[kLQ[5}h@3pH4j%&nk9g*Udc9d}qcdJk.lcb]ghgcZ,^UIgVxhf6b&fkUpc>dfvhk9+d4;glzcz$TLdkyNh1Pi<3laCnu1gJx_,Sk!mbg1V:ddx=iIyg6DcU*d_Ph~Zg:fkxajWX[9,XeykFdh%,e*fV4dnIKnh(k(#Xa_YK]W@8h,Wc$NZ.0^t<iA[RHrZ`m^|qd'v[N4dJQfCnWxU_Wv[)#gqwd*@e>+eO#[Z|]{cQ]k_*S[FRg{;gKXf_|d&?IOic,eg+lRO6e9Mg?vdSvd.)[_6bQniW;hil`v-]7Pd`pSQR`p3_|yUjPjs+o!5m(,`ygZ54a@Zh>:c'7WPi_Y]VTRl)A]M|fJZcMck#|jFvSP%bwOb6=fB/h.|aSddXHeM{cgQ]D!h2|d,0Tbacpmbtl^g0_Q2Zx'gRPXD]^S$]2_jm$dC`Z9fV;_`n1db|`!%daxU~AK#<g.Rg@2e'y^z-hcEY<AX`1d>jhUa`DWcf}g42cX!`R%oCpa7Fj9PgF,S}x].WM}Bj|2iJ(oR>h`;g-%TpXO_(m5OkIXa)uds|cyhhclh%La'EbJYQ3|VcNeh8iehg:%g%]d3pi3uc4MgW0ky9gBAjW9]|pgbZhZ@m0}jlC_gwajCe_Dc0QdhYcr+WJyjHdW$%YSfZJ!cxrcw_ig5R-)dh~lS6jN'h^ccQ{g:.g<Ol$nm-UgZ,_^>bnOfdndT0jFb`*}g@]j(_nMrdI$ao(f($he]gdSdATkE#bW>dS,cN0a2VbsCgXgkJKMTN[w!^wogP9c_5hbuhp9n?IY_%o$OhVGZ=EX_7l*Qe-qglYj,?lf_`ek[cYZ(mk^>gg&mda`L9qgm`FQii~_#9d2}hpVnXJlwJ_e>pomhiBdO6m-+gspc*WdQ(i0jl~~kDJgJ6l0dnP7n+~c9Ca0vV3@egRch%]12]/(d+DciCg^VcQ/i+zi$BlD7dr=ah?cZba[r[.Ukd0gaK`!zgU[ae&_aB_Qx]y3jKS`NGfE)ll1gqJi_sglhdj8cNOh+ljcraxZTY7fo&]+Eg>(da8h6Gb0hZPGV,}]hMh4Fdm(]mbnKbZ!~gGAhz^OXKfRsb_%Z_|b5a`sxfG3hp7jP5loZm)yig+ml*c*}cE{Wz]lFvh<]cZcdN=iBenW8aARj@^h6vcVCeNNfGshRzj~*aVLl3W_f7`A9hRai=vhf|[]8iH,kY:d<@aiah}*WuFdtoi{Fn=vo-NVwHh:fDljcyvkVXiHHR;1d^Q_],b;*b},b=}]X<JF(_|2cos_<:cN(gu^_dV[Y5_g!cRBoKTeeJd[_Tql^'$a_&_z|DS&gRmUl<c`Eg[u^PWk0C`E%U=mJr>`wT]u*b-:W/f_]Q`ZmUP=e;:h:!k]WZU3dx'dD$gy.g38fGUhobhZrYHgSa8epzgr?gOVh}<R68ZhcZV+f{Xc_YR~ebnOdZ!`Pwh5=a(7frzW/Akw(cG'ibba&4mEmi40i_3eO2`e&c9mkYFe]NlH^ja@l.Qkg/ZMKh*Ciunf>Oc48kaFmGTa]S[E+`i'Xh?X2AgRHghs^[~cv,e[Ji<aZ9@k&oaF'gILNjf`ejZaOi&'TbUc~PfJEa:gh+vfV*i/bW9.a8}dPa`odWZ!`&=kz&`GQj`y]Q~_LSb,|j<]bBzi7beefi$^nMfg`FfRB_D^`g,j]id0sVP>^#!^`6ezdggZZ62_p_Z+[iSdg%:WM+imhgAIbZjb62UrY`h1fblg}[^4&kOYbw.`F?leFlE:gwJdS=c#Vf-Y^BJf?afQA[rGbWSjL4dHUc''g&dZ55f|5g6]k8FiGt^P&h!:jg?iF:[=1k=G`eHg^}k.jiF7]O@g7!k8@jtPo6$g}=l;]lg&lReqo5ka/l'Tl0,ZR6kWJl^+gzYb|Zlkj^sRgxjdYraIUb`QOO(g$$k6r[W'RZ[e||aU{e<oXK2Z]KQloap'eXGWz(JE$e1]dzcc{wdh?_68m$Te%RX?L_#|itbhp]`U/nb^g}^j-fk}OlC<gB|dh-bG{aNjm8-_GTn><k&ihRDn_)kSYb@_kDejk4dHrht&h|El>:k6Mf}+dBOnRte}Tj{3j;miNvm$Gl1vhL@m2_a_Wd^^jKniCJjFEe]/h#3SSk`'ye+XctmfGo]'2g+,Z.xit)a0yg|Xl_B`/Pj5]h_zdiQijXete`*sW-8_GtcTF^z^k~nb`CgYVe1~h;Ro4Uaq`nuelLUos@a_t_R)m[tozibx@a[3jQ&eZYpoOd#Gi:JiXu_^:jsc_I=gi&^x[i]XmENjs7oh7Xkg_OJj=sb(rhU>e$OfEFfIuiT>c=GfZ.fRDkr&SFNmJ=i~Kia?h6T[92kZ3iRigEOG9%lT}iHhhB{c(jki>b&#hZ?m8qf.Vhrj`vZh]SnY)hDcPih_bcikYY=;m?$j`}gcyawSZ@UlW<kPIk>#hv3kK6h)(e&3l+^k:'ievj1$k55gDni!ZgEfg;}k):^r@]{c[EL[=TV&Dm_%l1qlwUmgBk=1]6Eky>h^bc.pmDFaR(oRNm%JsKOi:UkrFexPp^-fO4ku8f$!`8/k`/XRZgptlU[hwGS)<^b+g<^d&#j!$OerhR|OR[dVdg5,cx`kQubd.k`A]05h-ldEGhh!]JvhBhi!Jk]ag&AkK%hfRfBLS5}lehj<riHsgYPdq6jDwiHWg/,l%Gl%`gqpYn}br(]j&YB4dS)c9}lF>atpd-ohk4gJwj{UkfNa.xeFtbBVY(F`xKawFdCwfUyeL]m=1ir4c5;]h0hs4``wa2+c$-Z&Ug0<oBSj%zlM|cL/ezGdg`VShb&]d;dau!h,Ifyj`xPUx!j-gY0Jb,)N?VeCtlR1i6@j[-VuNnuB`w1i9Hc/ZM;Wa@AS]Fku9gw}k^{ePwmixk.`h`rkLmXJ1ay,jU,lM{ir!l*Smc~mhld(Tn,oZ&+d`en}id&3]CKda>kXpjN<kLo]o{l.Dos@jM/n*vTQ@cL:YGdkzulCn_'(XjFW/ajvUZx}[5@fo:lmsnNx]mhhc]Y;ygszi,$kpUVEcQyGnC-_{aof3cuki4Mgi}U@bfH-MS.hGn`Yoc}|h@,f))g+w_)sViiWr?i8Gj|*b=BdQ;g['i{FgPJf|8^eRcPfgk,de}eCHii[jk4e18iLugZieLed-Ra0}naVjBPaJ=kHDf{3h`IUe|gVmgE=]/#ioYo?0nw1bb9iYTkg^fFr[C~]'x[uyfUya9ld3mh-Wej<a!`g,%fA1iNzbUK]TXhkChWfY3Tfp.dB}]VMeHBS5sg>;ca:gO)g^`fL3h~fo)Cm6enDHoXKT];t$'c>%V0>nqukwYlV5n|1r@+f$Dp]@qbYsEac~!clAYlAd?LhV9W[!]S0g&2dNJdfwh@b`2Qh)}c,tfa6ZACawpd@SXHdhcBb|7k*yWL7gD;aT-`=f[%:gyyca?g99p/Be(-kexgYrhF$bGziHmk$9bb'mX)k~-qv[p_t_XuiANaU)mK3egSc6pn=+e0LmxtiIxmQMoiDki~[1<g{gcFbo`1h)i^xChaUbgpnb4oP%pU6r>HiVAkinoS%j{UlK1cohk7*l?;h/fm3vplcJSNoFnou'pMVW-%g;>eMxg3Lkgf`^:tWgr%#k,~kJ1qpSk)Kn]=g{Bc+6rDjsuzsV_XPhar'jj&rGw`3$vQhnzooszu;Lqz<ih&d63svZXMirTwr4gsW)uXlw_jp{xsfAtv1pMYo4Av7JxcHfbQq4#o#Dkrwh%Gll?k%}e:6nC&mi@mn0m`Ik_Gkwoottj+ao2ip*zkwNc^lmlTnk8uNVs>IlF]dMHqGPq-ao*3s7qr9kqm!u(~o7El}4oM,UYwpx'aiqui?p6krMznI7qzSvUVfE$m!=uj!o./v8Yl?'kKJbdoo#po3~iQeqGJu];kJaiAYkVioF1oSjr7*q(SkG|u'_[!`s+$o<;u=/upxk;MmPydqjlwVp`Ui^t[hvjL`s_9e?Pi`9s+0pB8ll&v/Tl~7k8ul`yt2Wv/6t(Bx-Go?HrhmcIJqv-sCpvsEl8Dtq<lV1wbtmwemC]`-Nn+bt:&s~Wt<,ulXrB}w{7oR&tf4ww:svHs+Cs3Ivw)sB;p.~h>4dd&qYKrw'j,hdrzu4)xnqlo`u^psJ<w;PoKWrM9wJoo{StLzoL(doFm00lrBgrXlV`n)ga5g]mY^!5nQits't)PlVXkMWn@3u[(qTWvR~j~NkoAjQbu>ElwWsunq+'ub7u:un1ReYZ_U*_])^fBlSglJ#_$ppj7gP5sM(^pwwI{ou(o#(_f#mAvor?kt4cWHt_hmTDu22dBTu]?sJ.smknxqhJ-o.Sp>ht&Gn=[p+XjEk^z8wBBZ=%t)BlUSgM]ulomr}r#Vpj}lI#vz#su&x`Csj;lwep8aw{+oo6r4}o_iw68gervB$w]Gw'8h;Our7wtKtRXvm]p'#rs%uX8sADhSWjjwu'Go(UljWbW6rAJrfIk~Nsh<u51rqcl~*r[mnmAs-nscjv}xmX{vu@mtagd+vdBrcArmGtZ_t<Epu5q@Xt>`m6@o5nvffxA>v?6x8hxAGswseeqm4qjcCkYki(Ui2%g$Kg.|gd>k^;l&#jMhi93kC}b^$jx3nu?qq't?vs/ZoW>uN'^@XnNdsJ1p^CoI;oT=qW@q3IoDIt4AnP=w*jU1-siklP:x.Pv|]sxen;YlDotxTxtGu|]vikhXZk1tp}OrfrsJ?fN~qy6q]2n#mnb*pAQcG=r>up^CtnKu@TtJ7l])shqg@DsC[ow~Xn*tl9s+Zq>|r;Ch1Em7#qFnuUapN>tnWt?_uq=k1/_]/t2`q'z_S*vo)qgHv27hKn[J-`x:xDsr1anaLs=pwpVj4=sWQvhOg',s'U`BUjq|oz0tc(oC1suVs?KQ>Erg:rx'gRKp5!oU.mKxgn1t8MfDEiAsp{rm'hkI*sZ8lo.tHDs~%k;ru%JtmDs|8pw_sH#wmeoiDn^6s3am7'n?{Z,8eJWk<Be*GikVce+jOG`^6iYdoyil-+e<|qjRo/ZpGXlZ7qLYsYUs(Kr/co}f`GHf[sn[uuyAq3Vs&mllEiXjn}@q5BqqEaeIr~rpo'nZsr|Le/{iBRm4PjWinB=nRXjr_i+B`//jHHg[9sjwqG7k7/ikGiu+kt0ixokC2tFujBze<^tu8nl]s>eu`am<4wBcrVtnTRcD,s@fp<)sr%q3ToDgn2spkJt3NueXt~Gm#Hs_6o}QfdGvE&t4ZsYakbup.`w9%vdTw%5uI0t|Mts<w5}rZhrTAt*>u*Ymclo$Cp_Eeg|oDfnAUloZihycstk]yoFco--n>ep10otQl9}hY?p'Nqu[sjJrTAgR^s'Eqf%m%YoC^rXOrZ)v7%tDmtvSrA'n;Qr(Hn&~kD__&8o&Vj4@p1Ne`=Rybhq&e6qn:io2|no][>clcVrd7X.TmQrz#_nUot`ps.iqW>iJ#j>ZbT;moIf[_iUgpU7mw$v1~o&0w=zs%pmb$w^Yo}Xt!YsV4t)*oEP^p7vL}pBXpxypl~jvvuz:soXsn/lmmpMSkwrqXhmR%i*Or0+j+3n-Ckq!mO&d5.`nU[bjYb7rTRZr1m1>s+*k1CgE/hlnc'Ch9^gd/f=Mq`'o..m?bkqtk39eD;sp5s}9c(qq|Pk0vXxLfw$lbrm<H`amd:4jD!d(.jytk(%m]'l00c$Tg]ObM+thQkY+h[0l>bk=elBgd}^d$ui@Fi,-l2rlVkiy%k|?_gCgZ>gE't;MpNQq]:p2Rjoiv+2ewHff~togr|Xq{vrn4xe;l7to&xdxEtr_rB3cL'rjyiMiPtOm>>rXCtTEoO&nY~o4_rjxf31lP>o3>v/O^<:lEcb@[o,Nl!FoH:i8ZkU!mt~o*(m0$iO*oWRm}skGRkP>l#Asf,qaVn4)sBZnzBh'Bg')j#>qY&X31lK'dk}qgUs=!q]Xgv@sJos0C^ajm2roihl}Bi<[q8}o@un(dh;``s%ek7qzq`,3nZ~qB(q$jr+o}^Ks}5tZ:nrUs{{t;1sS:qHlj%Pp+ZsBTr-Gu:)o%#lq:q@%n3=lg%j72n$(rVHmKQslhpC%sb:b/CeZ@o1,l?Ah5Sj4aX8>p?seocm";
int main(){
    CRNN_CTC_MODEL<ARG,ARG> model(cnn1_arg,cnn2_arg,cnn3_arg,rnn_arg);
    int h,w;
    std::cin>>h>>w>>h;
    std::array<std::vector<std::array<ARG,height>>,1> in;
    for(int i=0;i<h;i++){
        std::string s;
        std::cin>>s;
        in[0].resize(s.length());
        for(int j=0;j<s.length();j++){
            in[0][j][i].x=(s[j]=='#');
        }
    }
    model.forward_propagation(in);
    std::string exp;
    for(int i:model.ctc.result){
        exp+=char_list[i];
    }
    printf("%d\n",eval(exp));
    return 0;
}
s="""                                                                       
                                     .s9X&X3r         .,:.             
            .s5S1i,                 S#@ASh5X#B1 ,s3XHBBBB#Hr           
           S#@HHBM#B&3s,  ,;irs1h11H@H;     .X@M##A91i,..iB@r          
          3@M;   .;19AM#BBMMMBBHAB@@G      :. h@B         ,@M.         
         :@@;          ;si:.     8@A    .9H&AA8@@&8h. .,;i;H@i         
         h@X                    ,@@i    5@h:B#81r13H#HBBBBB#@8         
         h@X                    .#@i    ,9XMM,      G@S    ,5@H.       
         ;@@59,                  rM@3i:,:iG@A       9@HX;    9@1       
          G@@X.                   .5AHHHA&83#Xr,  :G@&h@S   .B@;       
         .A@h                         ..    :9AAA##G9891   iH@s        
         A@5                                   .:G@5    .s&@@#:        
        3@&                                       r&HAAABH9;r@B        
       .#@;                                         ,isi,    8@1       
       ;@B                                                ,;r8@BS5hh1i 
  ,is15G@M551;       .                          ..       .G&XH@#888995 
 ,9GGG8G@@G99s     ;HMA:                       8MMX.         1@3       
        X@5        S@@@h                      .#@@@,       5SH@&si;,   
      .i3@@835     ,S9h.        ,59993i        ,h5;        iX@#GGXX9,  
     rGX88@@h.                  1@8ssB@.                .. ;B#i        
          iH#5sSS.               rS335:                 :&M@@Bs,       
         ,sX@@@@S                                       sB@Xh9HB&3.    
       sXBA3;,h&#BGh;.                              ,1G#@&r    ,i1.    
       s1,      :5XB#MH&951i:,.             .,;rh9&M@#AS:              
                    ,ih9XHB####MBBHHHHHHHBBM###MB&85i.                 
                             ,:irs1hhhh11sri:.                         
                                                                       67"""
prev, prev_i = s[0], 0
begin = 'X'
for i in range(1, len(s)):
    if prev != s[i]:
        x=(i-prev_i)
        t = ""
        while x:
            y = x % 15
            x = x // 15
            t = t + chr(ord('Y')+y)
        if prev == '\n':
            prev = '!'
        if i-prev_i == 1:
            print(prev, end='')
        else:
            print(prev + t[::-1], end='')
        ### &->X, !->\n
        prev, prev_i = s[i], i
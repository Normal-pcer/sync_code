document.addEventListener('DOMContentLoaded', function() {
    const general = document.getElementById('general');
    const cannon = document.getElementById('cannon');
    const bullet = document.getElementById('bullet');
    const explosion = document.getElementById('explosion');
    const resultTitle = document.getElementById('resultTitle');
    const resultMessage = document.getElementById('resultMessage');
    const rematchBtn = document.getElementById('rematchBtn');
    const exitBtn = document.getElementById('exitBtn');

    // 开始动画序列
    startAnimation();

    function startAnimation() {
        // 初始状态
        general.style.opacity = '1';
        cannon.style.opacity = '1';
        
        // 1秒后发射炮弹
        setTimeout(() => {
            // 炮口闪光
            cannon.style.animation = 'cannonRecoil 0.3s ease-out';
            
            // 显示子弹并开始移动
            bullet.style.opacity = '1';
            bullet.style.animation = 'shoot 0.5s linear forwards';
            
            // 子弹击中后
            setTimeout(() => {
                // 显示爆炸效果
                explosion.style.opacity = '1';
                explosion.style.animation = 'explode 0.6s ease-out forwards';
                
                // 将倒下
                general.style.animation = 'fallDown 1s ease-out forwards';
                
                // 更新结果文本
                setTimeout(() => {
                    resultTitle.textContent = '红方胜利';
                    resultMessage.textContent = '炮击毙敌将！';
                }, 500);
                
            }, 500);
        }, 1000);
    }

    // 按钮事件
    rematchBtn.addEventListener('click', function() {
        // 重置动画
        general.style.animation = '';
        cannon.style.animation = '';
        bullet.style.animation = '';
        explosion.style.animation = '';
        
        // 重置元素状态
        general.style.opacity = '1';
        general.style.transform = 'translateY(0) rotate(0deg)';
        bullet.style.opacity = '0';
        bullet.style.transform = 'translateX(0)';
        explosion.style.opacity = '0';
        explosion.style.transform = 'scale(0)';
        
        // 重新开始动画
        setTimeout(startAnimation, 300);
    });

    exitBtn.addEventListener('click', function() {
        if (confirm('确定要退出游戏吗？')) {
            // 实际应用中可能是关闭窗口或返回首页
            console.log('退出游戏');
        }
    });
    
    // 暴露API供游戏主逻辑调用
    window.showChessResult = function(resultType) {
        // 可以根据不同结果类型显示不同动画
        // resultType: 'cannon', 'horse', 'chariot' 等
        startAnimation();
    };
});

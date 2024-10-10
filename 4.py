import os
from pydub import AudioSegment
# 读取目录中所有wav文件，转化为ogg文件，名为"秒数s_原名.ogg"
dir2 = r"C:\Users\norma\Desktop\phi_3.9.0\AudioClip"
for filename in os.listdir(dir2):
    if filename.endswith(".wav"):
        file_path = os.path.join(dir2, filename)
        audio = AudioSegment.from_wav(file_path)
        seconds = round(len(audio) / 1000)  # 获取音频长度（秒）
        new_filename = f"{seconds}s_{filename[:-4]}.ogg"
        new_file_path = os.path.join(dir2, new_filename)
        audio.export(new_file_path, format="ogg")  # 导出为ogg格式
        print(f"Converted {filename} to {new_filename}")
        # 删除原wav文件
        os.remove(file_path)

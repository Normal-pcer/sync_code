import os
from pydub import AudioSegment
# 读取目录中所有wav文件，重命名为"秒数s_原名.wav"
dir2 = input("dir: ")
for filename in os.listdir(dir2):
    if filename.endswith(".wav"):
        file_path = os.path.join(dir2, filename)
        audio = AudioSegment.from_wav(file_path)
        seconds = round(len(audio) / 1000)  # 获取音频长度（秒）
        new_filename = f"{seconds}s_{filename[:-4]}.wav"
        new_file_path = os.path.join(dir2, new_filename)
        # 重命名文件
        os.rename(file_path, new_file_path)

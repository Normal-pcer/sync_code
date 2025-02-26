import ctypes

def eject_cdrom():
    result = ctypes.windll.winmm.mciSendStringW("set CDAudio door open", None, 0, None)
    if result != 0:
        print("无法打开光驱")
    else:
        print("光驱已打开")

eject_cdrom()
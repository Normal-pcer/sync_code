import base64

# 读取 Base64 编码的文本
with open("1.in", "r") as file:
    encoded_data = file.read()

# 解码为二进制数据
decoded_data = base64.b64decode(encoded_data)

# 保存为 ZIP 文件
with open("output.zip", "wb") as zip_file:
    zip_file.write(decoded_data)

print("解码完成，文件已保存为 output.zip")

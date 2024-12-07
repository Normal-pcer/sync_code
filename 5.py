"""
提取Phigros谱面信息并写入文件名
"""

import os, json
os.chdir(os.path.dirname(__file__))

if __name__ == '__main__':
    index = 0
    for filename in os.listdir('.'):
        if filename.startswith('Chart_'):
            index += 1
            difficulty = filename.split('_')[1] # EZ, HD, IN, AT, Legacy, IN_Error
            try:
                originalId = filename.split('#')[1]
            except IndexError:
                originalId = ""
            
            chart: dict = json.load(open(filename, 'r', encoding='utf-8'))
            
            judgeLines: list = chart['judgeLineList']
            
            bpm: int = int(judgeLines[0]['bpm'])
            notesCount: int = 0
            
            for judgeLine in judgeLines:
                notesCount += len(judgeLine['notesBelow'])+len(judgeLine['notesAbove'])
            
            newName = "{}#Notes={:04d}#BPM={:03d}#{}.json".format(difficulty, notesCount, bpm, originalId)
            print(index, newName)
            json.dump(chart, open(os.path.join('output', newName), 'w', encoding='utf-8'))

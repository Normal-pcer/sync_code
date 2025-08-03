import re
import sys

def extract_headings(markdown_content):
    """Extract headings from markdown content."""
    headings = []
    pattern = re.compile(r'^(#+)\s+(.*)$', re.MULTILINE)
    for match in pattern.finditer(markdown_content):
        level = len(match.group(1))  # Number of '#' indicates heading level
        text = match.group(2).strip()
        headings.append((level, text))
    return headings

def main(input_file):
    """Main function to process the markdown file."""
    try:
        with open(input_file, 'r', encoding='utf-8') as file:
            content = file.read()
        headings = extract_headings(content)
        output_file = input_file.replace('.md', '_out.md')
        with open(output_file, 'w', encoding='utf-8') as file:
            for level, text in headings:
                file.write(f"{'  ' * (level - 1)}- {text}\n")
        print(f"Headings extracted and saved to {output_file}")
    except Exception as e:
        print(f"Error processing file: {e}")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python script.py <input_file.md>")
    else:
        main(sys.argv[1])
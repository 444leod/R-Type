#!/bin/bash

files=$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -not -path "./build/*")

if ! command -v clang-format-19 &> /dev/null; then
    clang_format_command="clang-format"
else
    clang_format_command="clang-format-19"
fi

format_issues=0
for file in $files; do
    diff=$($clang_format_command -style=file "$file" | diff "$file" -)
    if [ -n "$diff" ]; then
        echo "❌ $file needs formatting"
        echo "$diff"
        format_issues=1
    fi
done

if [ $format_issues -eq 1 ]; then
    echo "Some files need formatting. Please run clang-format."
    exit 1
else
    echo "✅ All files are properly formatted yipee"
fi

#!/bin/bash

build_dir="build"

files=$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -not -path "./build/*")

if ! command -v clang-tidy-19 &> /dev/null; then
    clang_tidy_command="clang-tidy"
else
    clang_tidy_command="clang-tidy-19"
fi

tidy_issues=0
for file in $files; do
    echo "Checking $file..."
    if ! $clang_tidy_command \
        -p="${build_dir}" \
        --extra-arg="-I${build_dir}" \
        --warnings-as-errors=* \
        "$file"; then
        tidy_issues=1
    fi
done

if [ $tidy_issues -eq 1 ]; then
    echo "❌ Some files have tidy issues."
    exit 1
else
    echo "✅ All files pass tidy checks!"
fi

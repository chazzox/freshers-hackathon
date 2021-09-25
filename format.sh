#!/bin/sh
for FILE in */**; do
	if [[ "$FILE" =~ \.(c|h)$ ]]; then
		clang-format -i $FILE
	fi
done
echo "$(git diff --name-only)"


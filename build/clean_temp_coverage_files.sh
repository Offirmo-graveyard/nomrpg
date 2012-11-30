# /bin/bash --debug

# Offirmo 2012/07/15
# This script cleans the coverage analysis files.
# This script is NOT MEANT TO BE USED MANUALLY.
# Tt is called from a custom cmake target
# with correct args.

echo "* cleaning old coverage results..."
# hat tip to http://stackoverflow.com/questions/2590794/gcov-warning-merge-mismatch-for-summaries
find . -name '*.gcda' | xargs rm 2>/dev/null
find . -name '*.gcov' | xargs rm 2>/dev/null

exit 0

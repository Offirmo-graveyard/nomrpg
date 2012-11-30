# /bin/bash --debug

# Offirmo 2012/07/15
# This script launches the coverage analysis.
# This script is NOT MEANT TO BE USED MANUALLY.
# Tt is called from a custom cmake target
# with correct args.


source_listing_file=$1
source_root=$2
object_root=$3
gcov_summary=gcov_output.txt

echo "*** Coverage analysis begin ***"
#echo "* cwd          : `pwd`"
#echo "* file listing : $source_listing_file"
#echo "* source root  : $source_root"
#echo "* object root  : $object_root"

echo "* Analysis in progress..."

OIFS=$IFS
IFS="
"

reset_coverage_variables()
{
	file_count=0
	echo "" > $gcov_summary
}
reset_coverage_variables


process_coverage_infos_for_file()
{
	local file_path=$1
	local file_relative_path=${file_path#$source_root}
	#echo "  * processing $file_relative_path"
	
	file_count=`expr $file_count + 1`
	
	local gcno_path="${object_root}${file_relative_path}.gcno"
	#echo "gcno = $gcno_path"
	
	# --source-prefix \"$source_root\"
	local ret
	local result
	#local cmd="gcov --object-file '${gcno_path}'  '${file_path}' | grep --color=never --after-context=1 \"File '$source_root\" | grep --invert-match \"\\-\\-\""
	#echo "  cmd = $cmd"
	local error_file;
	error_file=`mktemp`
	result=`gcov --object-file "${gcno_path}"  "${file_path}" 2> $error_file | grep --color=never --after-context=1 "File '$source_root" | grep --invert-match "\-\-"`
	ret=$?
	# was there an error message ?
	local error_msg=`cat $error_file`
	local ok=0
	if [[ -z $error_msg ]]; then
		# OK, no error
		ok=1
	else
		# there was an error message
		local test=`cat $error_file | grep ".gcno:cannot open graph file"`
		if [[ -n $test ]]; then
			# OK, this error is not serious (handled somewhere else)
			ok=1
		else
			# ? this error is unexpected
			echo "XXX error : $error_msg"
			# ok stays false
		fi
	fi

	#echo "  output = \n${result}"
	rm $error_file
	echo "${result}" >> $gcov_summary
}

source_listing=`cat $source_listing_file`
for file_path in $source_listing; do
	echo -en '.'  # to see progress
	IFS=$OIFS
	extension="${file_path##*.}"
	#echo $file_path
	#echo $extension
	case $extension in
	cpp)
		process_coverage_infos_for_file $file_path
		;;
	cxx)
		process_coverage_infos_for_file $file_path
		;;
	c)
		process_coverage_infos_for_file $file_path
		;;
	esac
done
echo ""

# OK shell was nice,
# but for doing a bit of arithmetic and move around, let's switch to python
mydir=`dirname $0`
$mydir/finish_coverage_analysis.py  gcov_output.txt  $source_listing_file

echo "*** Coverage analysis done ***"



#--object-directory directory
#--object-file file
#    Specify either the directory containing the gcov data files, or the object path name.
# The .gcno, and .gcda data files are searched for using this option.
# If a directory is specified, the data files are in that directory and named after the input file name, without its extension. If a file is specified here, the data files are named after that file, without its extension.

# gcov should be run with the current directory the same as that when you invoked the compiler.
#cd /Home/ut1kab/xp/utcov/test/build/build-gcc/bin

# --source-prefix /Home/ut1kab/xp/utcov/test/
#gcov --preserve-paths --object-file "/Home/ut1kab/xp/utcov/test/build/build-gcc/bin/CMakeFiles/ut_model_dir/app/models/base/object.cpp.gcno"  /Home/ut1kab/xp/utcov/test/app/models/base/object.cpp

#gcov --preserve-paths --object-file "CMakeFiles/ut_model_dir/app/models/base/object.cpp.gcno"  /Home/ut1kab/xp/utcov/test/app/models/base/object.cpp

# --preserve-paths
#gcov --object-file "CMakeFiles/ut_model.dir/app/models/base/object.cpp.gcno"  /Home/ut1kab/xp/utcov/test/app/models/base/object.cpp

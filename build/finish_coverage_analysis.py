#! /usr/bin/python
# -*- coding: utf-8 -*-

import sys # for accessing argc/argv
import fileinput # reading files
import re  # regular expressions
import os # to query and manipulate a lot of OS-related things
import subprocess # to be able to launch commands


def usage():
	print "usage :"
	print "   (internal use only)"
	print "   " + sys.argv[0] + " <gcov_preformated_output> <source_list>"
	sys.exit(1)


def count_lines(file_path):
	#cmd = 'cat "' + file_path + '" | wc -l'
	cmd = 'cat "' + file_path + '" | grep --invert-match --extended-regexp "^[:space:]*$" --count'
	#print "executing " + cmd
	child = subprocess.Popen(
			args = cmd,
			shell = True,
			#bufsize   = 0,
			#stdin     = subprocess.PIPE,
			stdout    = subprocess.PIPE,
			# XXX don't catch stderr so we see error immediately in the console (better for current usage)
			#stderr    = subprocess.PIPE,
			#close_fds = False # close_fds = close all existing fds before calling = False because we want to redirect them
		)
	child.wait()
	stdout = child.stdout.read()
	line_count = int(stdout)
	#print "    counted physical lines : " + repr(line_count)
	return line_count

def main_processing(coverage_results, source_list):
	
	# init results var
	results={}
	# 1st init from source list
	for line in fileinput.input(source_list):
		#print line
		results[line.strip()]={'line_count': 0, 'coverage': 0, 'seen': False, 'ignore': False}
	
	# then parse coverage results
	coverage_expected=False
	current_file=""
	# File '/srv/dev/wt/wt-xp/app/models/user-society/real_user.hpp'
	file_line_regexp = re.compile("File '(.+)'")
	# Lines executed:100.00% of 4
	cov_results_regexp = re.compile("Lines executed:([0-9\.]+)\% of (\d+)")
	for line in fileinput.input(coverage_results):
		line = line.strip()
		if len(line) == 0:
			pass
		elif file_line_regexp.match(line):
			if coverage_expected:
				raise OSError("Unexpected file line !")
			m = file_line_regexp.match(line)
			current_file = m.group(1)
			#print "file line : " + current_file
			coverage_expected=True
		elif cov_results_regexp.match(line):
			if not coverage_expected:
				raise OSError("Unexpected result line !")
			m = cov_results_regexp.match(line)
			coverage = float(m.group(1))
			line_count = int(m.group(2))
			#print "result : '" + repr(coverage) + "'% of '" + repr(line_count) + "' lines for file " + current_file
			results[current_file]['seen'] = True
			if results[current_file]['coverage'] >= coverage:
				# nothing to do
				pass
			else:
				results[current_file]['coverage'] = coverage
				results[current_file]['line_count'] = line_count
			coverage_expected=False
		else:
			raise OSError("line not recognized !")
		pass

	# now do a post-processing
	# to handle files that were not seen
	for current_file in results.keys():
		if not results[current_file]['seen']:
			base_filename = os.path.basename(current_file)
			# This file was never seen in coverage results !
			#print "  - never seen : " + base_filename
			# we'll now use a heuristic to decide if it's normal or not
			splitext = os.path.splitext(base_filename)
			root      = splitext[0]
			extension = splitext[1]
			#print "extension = " + extension
			line_count = count_lines(current_file)
			if extension in [ '.c', '.cpp', '.cxx', '.cc', '.C' ]:
				# This is an implementation file.
				# We should have infos.
				# We'll use raw line count to evaluate logical line count
				# and keep a coverage of 0%
				# this file is now considered having 0% coverage of all its lines
				if line_count <= 1:
					# empty impl file, ignore it
					results[current_file]['ignore'] = True
					#print "    ignore."
				else:
					results[current_file]['line_count'] = line_count
			elif extension in [ '.h', '.hpp', '.hxx', '.hh', '.H' ]:
				# this is a header file.
				# It MAY or MAY NOT contain code...
				# We'll use heuristic to decide :
				# - if this header file has a corresponding implementation file,
				#   then we ignore this header (supposedly included in the impl file)
				translit = {'.h':'.c', '.hpp':'.cpp', '.hxx':'.cxx', '.hh':'.cc', '.H':'.C' }
				impl_file = os.path.dirname(current_file) + '/' + root + translit[extension]
				#print "    checking " + impl_file
				if line_count == 0:
					#print "    empty file, ignoring"
					results[current_file]['ignore'] = True
					pass
				elif impl_file in results.keys():
					#print "    There is a corresponding impl file. Ignore the header."
					results[current_file]['ignore'] = True
				else:
					#print "Will report header as never seen : " + current_file
					results[current_file]['line_count'] = line_count
					pass

	# sum results
	total_line_count=0
	total_line_passed=0.0
	# aggregate all results
	for current_file in results.keys():
		total_line_count += results[current_file]['line_count']
		total_line_passed += results[current_file]['line_count'] * results[current_file]['coverage'] / 100.
	print "* total executable lines :  " + repr(total_line_count)
	# now display executed lines.
	# if no decimal part greater than 1/100, we round it
	if int(total_line_passed*100) == int(total_line_passed)*100:
		# OK, small decimal part, round it
		print "* total executed lines   :  %i" % total_line_passed
	else:
		# quite big decimal part, keep it
		print "* total executed lines   :  %.2f   <-- messy because of the way gcov handle limits" % total_line_passed
	# now compute the coverage as percentage
	total_coverage = total_line_passed * 100 / total_line_count
	# display a comment depending on the %. Because it's fun.
	if total_coverage >= 100:
		coverage_comment = "Impossible..."
	elif total_coverage >= 90:
		coverage_comment = "Excellent !"
	elif total_coverage >= 70:
		coverage_comment = "Great !"
	else:
		coverage_comment = "Well, at last you're trying !"
	print "* total coverage         :  %.2f%%   <-- %s" % (total_coverage, coverage_comment)
	
	print "* Low coverage file(s) :"
	# sort files by coverage result
	sorted_hash = sorted(results, key=lambda key: results[key]['coverage'])
	for current_file in sorted_hash:
		# note : the True/False are for easy switch when testing
		if results[current_file]['ignore']:
			# this file is to be ignored
			pass
		elif True and results[current_file]['line_count'] == 0:
			# no executable lines : no problem if 0
			pass
		elif True and results[current_file]['coverage'] == 100:
			# full coverage, OK
			pass
		else:
			seen_msg = ''
			if not results[current_file]['seen']:
				seen_msg = '(never seen)'
			print "  %50s :  %5.2f%% of %4i %s" % (os.path.basename(current_file), results[current_file]['coverage'], results[current_file]['line_count'], seen_msg )

if __name__ == "__main__":
	if len(sys.argv) < 3:
		usage() # will exit
	#print repr(sys.argv)
	main_processing(sys.argv[1], sys.argv[2])

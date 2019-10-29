#!/usr/bin/perl -l -n
#
#     Clear Linux Project for Intel Architecture Power tweaks
#
#       Copyright (C) 2018 Intel Corporation
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, version 3 of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Authors:
#          Thiago Macieira <thiago.macieira@intel.com>
#use strict;
#my $printing;
BEGIN {
    print <<EOF;
#include "clr_power.h"
int generated_tweaks(void)
{
        int status = 0;
EOF
}
END {
    print <<EOF;

        return status;
}
EOF
}

$printing = 1, next if (/--BEGIN--/);
next unless $printing;
print, next if (/^$/);

if (/^# (.*)/) {
    printf("\t// %s\n", $1);
    next;
}

@_ = split /\s+/;
die ("Unsupported line: $_")
    unless $_[0] eq "w" && $_[2] eq "-" && $_[3] eq "-" && $_[4] eq "-" && $_[5] eq "-";
my $function = 'write_string_to_file';
$function = 'write_string_to_files' if $_[1] =~ /\*/;
printf("\tstatus |= %s(\"%s\", \"%s\");\n", $function, $_[1], $_[6]);

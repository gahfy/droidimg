TESTS = files_read_no_permission_before.sh
TESTS += files_read_no_permission
TESTS += files_read_no_permission.sh
TESTS += files_read_not_existing
TESTS += files_read_not_existing.sh
TESTS += files_read_empty_content_before.sh
TESTS += files_read_empty_content
TESTS += files_read_empty_content_after.sh
TESTS += files_write_no_permission_before.sh
TESTS += files_write_no_permission
TESTS += files_write_no_permission.sh
TESTS += files_write_no_permission_directory_before.sh
TESTS += files_write_no_permission_directory
TESTS += files_write_no_permission_directory.sh

XFAIL_TESTS = files_read_no_permission
XFAIL_TESTS += files_read_not_existing
XFAIL_TESTS += files_write_no_permission
XFAIL_TESTS += files_write_no_permission_directory

check_PROGRAMS = files_read_no_permission
check_PROGRAMS += files_read_not_existing
check_PROGRAMS += files_read_empty_content
check_PROGRAMS += files_write_no_permission
check_PROGRAMS += files_write_no_permission_directory

files_read_no_permission_SOURCES = files_read_no_permission.c
files_read_no_permission_SOURCES += ${SRCDIR}/files/commons.c
files_read_no_permission_SOURCES += ${SRCDIR}/errors/errors.c
files_read_no_permission_SOURCES += ${SRCDIR}/utils/string_utils.c
files_read_no_permission_SOURCES += ${SRCDIR}/logging/logging.c

files_read_not_existing_SOURCES = files_read_not_existing.c
files_read_not_existing_SOURCES += ${SRCDIR}/files/commons.c
files_read_not_existing_SOURCES += ${SRCDIR}/errors/errors.c
files_read_not_existing_SOURCES += ${SRCDIR}/utils/string_utils.c
files_read_not_existing_SOURCES += ${SRCDIR}/logging/logging.c

files_read_empty_content_SOURCES = files_read_empty_content.c
files_read_empty_content_SOURCES += ${SRCDIR}/files/commons.c
files_read_empty_content_SOURCES += ${SRCDIR}/files/reader.c
files_read_empty_content_SOURCES += ${SRCDIR}/errors/errors.c
files_read_empty_content_SOURCES += ${SRCDIR}/utils/string_utils.c
files_read_empty_content_SOURCES += ${SRCDIR}/logging/logging.c

files_write_no_permission_SOURCES = files_write_no_permission.c
files_write_no_permission_SOURCES += ${SRCDIR}/files/commons.c
files_write_no_permission_SOURCES += ${SRCDIR}/errors/errors.c
files_write_no_permission_SOURCES += ${SRCDIR}/utils/string_utils.c
files_write_no_permission_SOURCES += ${SRCDIR}/logging/logging.c

files_write_no_permission_directory_SOURCES = files_write_no_permission.c
files_write_no_permission_directory_SOURCES += ${SRCDIR}/files/commons.c
files_write_no_permission_directory_SOURCES += ${SRCDIR}/errors/errors.c
files_write_no_permission_directory_SOURCES += ${SRCDIR}/utils/string_utils.c
files_write_no_permission_directory_SOURCES += ${SRCDIR}/logging/logging.c
USE_CCACHE = 0
ARSDK_BUILD_FOR_APP = 1

# http://stackoverflow.com/questions/10858261/abort-makefile-if-variable-not-set
#
# Check that given variables are set and all have non-empty values,
# die with an error otherwise.
#
# Params:
#   1. Variable name(s) to test.
#   2. (optional) Error message to print.
check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $(value 2), ($(strip $2)))))

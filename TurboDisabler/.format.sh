#/bin/sh

rm *.orig
astyle -A9 \
--pad-oper \
--pad-comma \
--pad-header \
--unpad-paren \
--mode=c \
--convert-tabs \
`ls *.cpp *.hpp *.c *.h `



rm *.orig

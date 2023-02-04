CONFIG(debug,debug|release){
COMPILE = debug
} else {
COMPILE = release
}

BUILD_PATH      = $$ROOT_PATH/out
LIBS_PATH       = $$BUILD_PATH/$$COMPILE/lib
OUTPUT_PATH     = $$BUILD_PATH/$$COMPILE/bin

if(contains(TEMPLATE, lib)){
    DESTDIR = $$LIBS_PATH
    DLLDESTDIR = $$OUTPUT_PATH
}else{
    DESTDIR = $$OUTPUT_PATH
}

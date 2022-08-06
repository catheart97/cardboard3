TEMPLATE = app

TARGET = Cardboard3

QT = \
    core \
    gui \
    widgets \
    opengl

## Uncomment to use orientation sensor on android.
## Not tested, cause my phone does not seem to support these
#DEFINES += USE_SENSORS
#QT += sensors

CONFIG += c++17 # uses std::optional

HEADERS = \
    $$PWD/include/Window.h \
    include/My/CubeObject.h \
    include/My/DistortionMaterial.h \
    include/My/Material.h \
    include/My/Object.h \
    include/My/OpenGLResolver.h \
    include/My/PBRMaterial.h \
    include/My/PhongMaterial.h \
    include/My/QuadObject.h \
    include/My/Scene.h \
    include/My/SkyBoxObject.h

SOURCES = \
    $$PWD/src/Window.cpp \
    $$PWD/src/My/CubeObject.cpp \
    $$PWD/src/main.cpp \
    src/My/SkyBoxObject.cpp \
    src/My/DistortionMaterial.cpp \
    src/My/PBRMaterial.cpp \
    src/My/PhongMaterial.cpp \
    src/My/QuadObject.cpp \
    src/My/Scene.cpp

INCLUDEPATH += \
    include/My \
    include

RESOURCES += \
    res/res.qrc

target.path = $$PWD
INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    res/cubemap/back.png \
    res/cubemap/down.png \
    res/cubemap/front.png \
    res/cubemap/left.png \
    res/cubemap/right.png \
    res/cubemap/up.png \
    res/shader/PBRFragment.glsl \
    res/shader/PBRVertex.glsl \
    res/shader/PhongFragment.glsl \
    res/shader/PhongVertex.glsl \
    res/shader/DistortionFragment.glsl \
    res/shader/DistortionVertex.glsl \ \
    res/shader/SkyboxFragment.glsl \
    res/shader/SkyboxVertex.glsl

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

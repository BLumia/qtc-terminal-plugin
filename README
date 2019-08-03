# Terminal Plugin for QtCreator

based on @manyoso 's [qt-creator-terminalplugin](https://github.com/manyoso/qt-creator-terminalplugin), with some hack to fit my own requirement.

## Build:

I did my work under Deepin but the following steps should works under all debian based system.

Require `qtermwidget` (on my system it's `libqtermwidget5-0-dev`) installed, also require the qtcreator(on my system it's just called `qtcreator`) installed.

Use `apt source qtcreator` to get the source code of QtCreator.

Either set the `IDE_SOURCE_TREE` when running qmake, or set the `QTC_SOURCE` environment variable, or by edit the `myproject.pro` and set it to the currect QtCreator source location.

If you are under a normal amd64 system, `IDE_BUILD_TREE` can remain `/usr/lib/x86_64-linux-gnu/qtcreator` without modification, or you should set `IDE_BUILD_TREE` or `QTC_BUILD` environment variable to the currect path.

Then do your normal `qmake` step :)

```
$ cd /path/to/the/plugin/source/folder/
$ mkdir build
$ cd build
$ qmake ../
$ make
```

Then it will be built under `~/.local/share/data/QtProject/qtcreator/plugins/<qtcreator-version>/`
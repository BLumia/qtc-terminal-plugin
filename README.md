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

## Quick Start:

After installation (put the compiled `libPineappleTerminal.so` to your `~/.local/share/data/QtProject/qtcreator/plugins/<qtcreator-version>/` folder *OR* somewhere else like `/usr/lib/x86_64-linux-gnu/qtcreator/plugins`, you will see a Terminal pane in the Output Panes area after you launch QtCreator.

### Shortcuts:

By default it's <kbd>Ctrl+Shift+C</kbd> to copy and <kbd>Ctrl+Shift+V</kbd> to paste. You can [update the key binding via QtCreator's setting dialog](https://doc.qt.io/qtcreator/creator-keyboard-shortcuts.html) at `Tools > Options > Environment > Keyboard`. By filting with `PineappleTerminal` you can see the shortcuts provided by this plugin.

Please notice currently you need restart QtCreator to make the shortcut change apply.

### Context Menu:

You should already know what does `Copy` and `Paste` means. Other than that, there is a `Change Directory` menu entry, which means `cd` to current project source directory. If there are no project opened, it will `cd` to the directory which current opend file located at. If still no file opened, it will simply `cd` to current working dir (nearly meaningless action).

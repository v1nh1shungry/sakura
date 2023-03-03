# Sakura Engine

Sakura Engine is a simple visual novel engine for educational purpose. It is still in heavy development, and should not be used in production.

# Quick Start

For now Sakura Engine doesn't provide any pre-built binary. So you should build it from source by yourself.

## Build

Sakura Engine uses [xmake](https://xmake.io/) as the build system :thumbsup: You should first install xmake, see more details about installing xmake in your platform :point_right: https://xmake.io/#/guide/installation.

Clone this project and just run `xmake`.

```bash
git clone https://github.com/v1nh1shungry/sakura.git
cd sakura
xmake
```

# Example

There is a simple demo [杰哥不要啊～](examples/%E6%9D%B0%E5%93%A5%E4%B8%8D%E8%A6%81%E5%95%8A~/) :)

I removed all the resources (textures, music, font and so on), so this example cannot work. You can check these files as a reference.

## Screenshots

![杰哥不要啊～ 1](docs/assets/杰哥不要啊1.png)

![杰哥不要啊～ 2](docs/assets/杰哥不要啊2.png)

# Documentation

All documentation can be found in [docs](docs/) folder.

See:

* The `Elaina` script language reference in [elaina.md](docs/elaina.md)

# TODO

Development work is still far from complete.

- [ ] Documentation
- [ ] Voice
- [ ] Font config
- [ ] Archive
- [ ] Packaging project
- [ ] Encryption
- [ ] Visualization Development
- [ ] IDE support / Visual Studio Code extension

# Acknowledgement

Sakura Engine depends on the following projects. Thanks :thumbsup:

* [xmake](https://github.com/xmake-io/xmake.git)
* [sfml](https://github.com/SFML/SFML.git)
* [fmt](https://github.com/fmtlib/fmt.git)
* [magic_enum](https://github.com/Neargye/magic_enum.git)
* [nlohmann_json](https://github.com/nlohmann/json.git)

<div align="center">
  <h1>dexbox</h1>

  <div>
    <a href="https://github.com/tboox/dexbox/actions?query=workflow%3Abuild">
      <img src="https://img.shields.io/github/workflow/status/tboox/dexbox/build/master.svg?style=flat-square" alt="github-ci" />
    </a>
    <a href="https://github.com/tboox/dexbox/releases">
      <img src="https://img.shields.io/github/release/tboox/dexbox.svg?style=flat-square" alt="Github All Releases" />
    </a>
    <a href="https://github.com/tboox/dexbox/blob/master/LICENSE.md">
      <img src="https://img.shields.io/github/license/tboox/dexbox.svg?colorB=f48041&style=flat-square" alt="license" />
    </a>
  </div>
  <div>
    <a href="https://www.reddit.com/r/tboox/">
      <img src="https://img.shields.io/badge/chat-on%20reddit-ff3f34.svg?style=flat-square" alt="Reddit" />
    </a>
    <a href="https://gitter.im/tboox/tboox?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge">
      <img src="https://img.shields.io/gitter/room/tboox/tboox.svg?style=flat-square&colorB=96c312" alt="Gitter" />
    </a>
    <a href="https://t.me/tbooxorg">
      <img src="https://img.shields.io/badge/chat-on%20telegram-blue.svg?style=flat-square" alt="Telegram" />
    </a>
    <a href="https://jq.qq.com/?_wv=1027&k=5hpwWFv">
      <img src="https://img.shields.io/badge/chat-on%20QQ-ff69b4.svg?style=flat-square" alt="QQ" />
    </a>
    <a href="https://xmake.io/#/sponsor">
      <img src="https://img.shields.io/badge/donate-us-orange.svg?style=flat-square" alt="Donate" />
    </a>
  </div>

  <p>A lightweight dex file parsing library</p>
</div>

## Introduction ([中文](/README_zh.md))

dexbox is a lightweight dex file parsing library.

If you want to know more, please refer to:

* [Documents](https://xmake.io/#/home)
* [HomePage](https://xmake.io)
* [Github](https://github.com/tboox/dexbox)
* [Gitee](https://gitee.com/tboox/dexbox)

## Prerequisites

XMake installed on the system. Available [here](https://github.com/xmake-io/xmake).

## Build

```console
$ xmake
```

## Run tests

```console
$ xmake run dexdump tests/tests.dex
.file _cast.java
.class _cast
.super Object
    .method public void <init>()
        .register 1
        .argument 1
        .catches 0
        .prologue
        0x000b0c: 7010 4e00 0000                         |0000: invoke-direct {v0}, Object.<init>()void
        0x000b12: 0e00                                   |0003: return-void

    .method public int main(java.lang.String[])
        .register 7
        .argument 1
        .catches 0
        .prologue
        0x000b24: 6200 0500                              |0000: sget-object v0, System.out:PrintStream
        0x000b28: 1a01 a800                              |0002: const-string v1, "test: ==============================================================="
        0x000b2c: 6e20 4800 1000                         |0004: invoke-virtual {v0, v1}, PrintStream.println(String)void
        0x000b32: 6200 0500                              |0007: sget-object v0, System.out:PrintStream
        0x000b36: 1a01 a900                              |0009: const-string v1, "test: cast: .."
        0x000b3a: 6e20 4800 1000                         |000b: invoke-virtual {v0, v1}, PrintStream.println(String)void
        0x000b40: 6200 0500                              |000e: sget-object v0, System.out:PrintStream
        0x000b44: 1a01 0000                              |0010: const-string v1, ""
        0x000b48: 6e20 4800 1000                         |0012: invoke-virtual {v0, v1}, PrintStream.println(String)void
        0x000b4e: 12f0                                   |0015: const/4 v0, #-1 // #ff
        0x000b50: 8102                                   |0016: int-to-long v2, v0
        0x000b52: 6201 0500                              |0017: sget-object v1, System.out:PrintStream
        0x000b56: 2204 2400                              |0019: new-instance v4, StringBuilder
        0x000b5a: 7010 5100 0400                         |001b: invoke-direct {v4}, StringBuilder.<init>()void
        0x000b60: 1a05 9d00                              |001e: const-string v5, "num = "
        0x000b64: 6e20 5600 5400                         |0020: invoke-virtual {v4, v5}, StringBuilder.append(String)StringBuilder
        0x000b6a: 0c04                                   |0023: move-result-object v4
        0x000b6c: 6e30 5500 2403                         |0024: invoke-virtual {v4, v2, v3}, StringBuilder.append(long)StringBuilder
        0x000b72: 0c02                                   |0027: move-result-object v2
        0x000b74: 6e10 5700 0200                         |0028: invoke-virtual {v2}, StringBuilder.toString()String
        0x000b7a: 0c02                                   |002b: move-result-object v2
        0x000b7c: 6e20 4800 2100                         |002c: invoke-virtual {v1, v2}, PrintStream.println(String)void
        0x000b82: 8f01                                   |002f: int-to-short v1, v0
        0x000b84: 6202 0500                              |0030: sget-object v2, System.out:PrintStream
        0x000b88: 2203 2400                              |0032: new-instance v3, StringBuilder
        0x000b8c: 7010 5100 0300                         |0034: invoke-direct {v3}, StringBuilder.<init>()void
        0x000b92: 1a04 9d00                              |0037: const-string v4, "num = "
        0x000b96: 6e20 5600 4300                         |0039: invoke-virtual {v3, v4}, StringBuilder.append(String)StringBuilder
        0x000b9c: 0c03                                   |003c: move-result-object v3
        0x000b9e: 6e20 5400 1300                         |003d: invoke-virtual {v3, v1}, StringBuilder.append(int)StringBuilder
        0x000ba4: 0c01                                   |0040: move-result-object v1
        0x000ba6: 6e10 5700 0100                         |0041: invoke-virtual {v1}, StringBuilder.toString()String
        0x000bac: 0c01                                   |0044: move-result-object v1
        0x000bae: 6e20 4800 1200                         |0045: invoke-virtual {v2, v1}, PrintStream.println(String)void
        0x000bb4: 8e01                                   |0048: int-to-char v1, v0
        0x000bb6: 6202 0500                              |0049: sget-object v2, System.out:PrintStream
        0x000bba: 2203 2400                              |004b: new-instance v3, StringBuilder
        0x000bbe: 7010 5100 0300                         |004d: invoke-direct {v3}, StringBuilder.<init>()void
        0x000bc4: 1a04 9d00                              |0050: const-string v4, "num = "
        0x000bc8: 6e20 5600 4300                         |0052: invoke-virtual {v3, v4}, StringBuilder.append(String)StringBuilder
        0x000bce: 0c03                                   |0055: move-result-object v3
        0x000bd0: 6e20 5400 1300                         |0056: invoke-virtual {v3, v1}, StringBuilder.append(int)StringBuilder
        0x000bd6: 0c01                                   |0059: move-result-object v1
        0x000bd8: 6e10 5700 0100                         |005a: invoke-virtual {v1}, StringBuilder.toString()String
        0x000bde: 0c01                                   |005d: move-result-object v1
        0x000be0: 6e20 4800 1200                         |005e: invoke-virtual {v2, v1}, PrintStream.println(String)void
        0x000be6: 8d00                                   |0061: int-to-byte v0, v0
        0x000be8: 6201 0500                              |0062: sget-object v1, System.out:PrintStream
        0x000bec: 2202 2400                              |0064: new-instance v2, StringBuilder
        0x000bf0: 7010 5100 0200                         |0066: invoke-direct {v2}, StringBuilder.<init>()void
        0x000bf6: 1a03 9d00                              |0069: const-string v3, "num = "
        0x000bfa: 6e20 5600 3200                         |006b: invoke-virtual {v2, v3}, StringBuilder.append(String)StringBuilder
        0x000c00: 0c02                                   |006e: move-result-object v2
        0x000c02: 6e20 5400 0200                         |006f: invoke-virtual {v2, v0}, StringBuilder.append(int)StringBuilder
        0x000c08: 0c00                                   |0072: move-result-object v0
        0x000c0a: 6e10 5700 0000                         |0073: invoke-virtual {v0}, StringBuilder.toString()String
        0x000c10: 0c00                                   |0076: move-result-object v0
        0x000c12: 6e20 4800 0100                         |0077: invoke-virtual {v1, v0}, PrintStream.println(String)void
        0x000c18: 1600 ffff                              |007a: const-wide/16 v0, #-1 // #ffff
        0x000c1c: 6202 0500                              |007c: sget-object v2, System.out:PrintStream
        0x000c20: 2203 2400                              |007e: new-instance v3, StringBuilder
        0x000c24: 7010 5100 0300                         |0080: invoke-direct {v3}, StringBuilder.<init>()void
        0x000c2a: 1a04 9d00                              |0083: const-string v4, "num = "
        0x000c2e: 6e20 5600 4300                         |0085: invoke-virtual {v3, v4}, StringBuilder.append(String)StringBuilder
        0x000c34: 0c03                                   |0088: move-result-object v3
        0x000c36: 8400                                   |0089: long-to-int v0, v0
        0x000c38: 6e20 5400 0300                         |008a: invoke-virtual {v3, v0}, StringBuilder.append(int)StringBuilder
        0x000c3e: 0c00                                   |008d: move-result-object v0
        0x000c40: 6e10 5700 0000                         |008e: invoke-virtual {v0}, StringBuilder.toString()String
        0x000c46: 0c00                                   |0091: move-result-object v0
        0x000c48: 6e20 4800 0200                         |0092: invoke-virtual {v2, v0}, PrintStream.println(String)void
        0x000c4e: 6200 0500                              |0095: sget-object v0, System.out:PrintStream
        0x000c52: 1a01 0000                              |0097: const-string v1, ""
        0x000c56: 6e20 4800 1000                         |0099: invoke-virtual {v0, v1}, PrintStream.println(String)void
        0x000c5c: 6200 0500                              |009c: sget-object v0, System.out:PrintStream
        0x000c60: 1a01 aa00                              |009e: const-string v1, "test: cast: ok"
        0x000c64: 6e20 4800 1000                         |00a0: invoke-virtual {v0, v1}, PrintStream.println(String)void
        0x000c6a: 1200                                   |00a3: const/4 v0, #0 // #0
        0x000c6c: 0f00                                   |00a4: return v0
```

## Contacts

* Email：[waruqi@gmail.com](mailto:waruqi@gmail.com)
* Homepage：[tboox.org](https://tboox.org)
* Community：[/r/tboox on reddit](https://www.reddit.com/r/tboox/)
* ChatRoom：[Char on telegram](https://t.me/tbooxorg), [Chat on gitter](https://gitter.im/tboox/tboox?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
* QQ Group: 343118190(full), 662147501
* Wechat Public: tboox-os

## Backers

Thank you to all our backers! 🙏 [[Become a backer](https://opencollective.com/xmake#backer)]

<a href="https://opencollective.com/xmake#backers" target="_blank"><img src="https://opencollective.com/xmake/backers.svg?width=890"></a>

## Sponsors

Support this project by becoming a sponsor. Your logo will show up here with a link to your website. [[Become a sponsor](https://opencollective.com/xmake#sponsor)]

<a href="https://opencollective.com/xmake/sponsor/0/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/0/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/1/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/1/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/2/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/2/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/3/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/3/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/4/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/4/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/5/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/5/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/6/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/6/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/7/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/7/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/8/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/8/avatar.svg"></a>
<a href="https://opencollective.com/xmake/sponsor/9/website" target="_blank"><img src="https://opencollective.com/xmake/sponsor/9/avatar.svg"></a>



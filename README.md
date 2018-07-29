# STM32-W5500-test
test of W5500 on STM32, testing on STM32F103C8T6 with STDPERIPH library v3.5 (preparing for PassiveVLC project)

## Motivation

PassiveVLC工程使用的古老版本的STM32标准外设库，与目前`CubeMX`主流的HAL库和LL库完全不兼容，经过暴力搜索到的STM32F10x标准外设库，经过一通魔改，终于获得了一个干净的项目工程！

## W5500库植入

从W5500官网上下载demo，迁移过来文件，全部放置到`W5500Lib`文件夹下
# GAS 学习资源调研报告

> 面向：会少量 C# 的游戏策划。目标：魂类 ARPG 求职 demo（UE5.8）。
> 一句话结论：现实路径是「工程转 C++ → 照 tranek 文档 / Narxim 示例搭最小 GAS 骨架（C++ 只写一次，大量可抄）→ 之后所有技能、效果、数值都在蓝图里做」。官方 Combat 模板不用 GAS；官方 GAS 参考工程是 Lyra，门槛高，后期再看。

## 1. tranek/GASDocumentation（社区最权威指南）

- URL：https://github.com/tranek/GASDocumentation （6k+ 星，MIT，社区"圣经"+ 示例工程）
- 章节结构：1 简介 → 2 示例工程 → 3 项目接入步骤 → 4 核心概念（4.1 ASC / 4.2 GameplayTag / 4.3 Attribute / 4.4 AttributeSet / 4.5 GE / 4.6 GA / 4.7 AbilityTask / 4.8 GameplayCue / 4.9 AbilitySystemGlobals / 4.10 Prediction）→ 5 调试 → 6 常见坑 → 7 QA → 8 优化 → 9 排查。
- 最小接入：启用 GameplayAbilities 插件 → Build.cs 加 `GameplayAbilities, GameplayTags, GameplayTasks` → 重新生成工程文件。
- ASC 挂哪：多人标准做法是玩家英雄挂 PlayerState、AI 挂 Character；单机 demo 全挂 Character 即可（我们采用）。
- AttributeSet 只能 C++：`ATTRIBUTE_ACCESSORS` 宏 + `PreAttributeChange`（钳制）+ `PostGameplayEffectExecute`（扣血后处理）。
- 引擎版本：主分支对齐 UE5.3，5.4–5.8 通常直接升级可用，核心 API 未大变。
- 读法：先 §1/§3/§4.1–4.6，第 4 章读两遍。全程案头参考书。
- 姊妹进阶工程：https://github.com/tranek/GASShooter

## 2. Epic 官方资源

- GAS 官方文档（概念名词表，无搭建教程）：https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-ability-system-for-unreal-engine
- 官方入门教程「Your First 60 Minutes with GAS」（第 1 周热身）：https://dev.epicgames.com/community/learning/tutorials/8Xn9/unreal-engine-epic-for-indies-your-first-60-minutes-with-gameplay-ability-system
- Lyra Starter Game（官方 GAS 架构示范，门槛高，后期选择性借鉴）：https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-starter-game-in-unreal-engine

## 3. UE5.6+ 官方 Combat 模板与 GAS 的关系

- 公告：https://www.unrealengine.com/en-US/news/updated-game-templates-for-unreal-engine-5-6available-now
- Variants 文档：https://dev.epicgames.com/documentation/unreal-engine/variants-in-game-templates?lang=en-US
- 结论：Variant_Combat **不用 GAS**（与本机侦察一致）。可拆它的连招、StateTree AI 参考，但不以它为底。

## 4. 精选开源/专项资源

1. **Narxim/Narxim-GAS-Example** — https://github.com/Narxim/Narxim-GAS-Example
   最小 GAS 骨架工程（已对齐 UE5.6）：生命/体力属性、伤害与抗性 ExecutionCalculation、UI 绑定示例。第 1–2 周对照抄写。
2. **Voidware-Prohibited/TargetSystemPlugin** — https://github.com/Voidware-Prohibited/TargetSystemPlugin
   黑魂式锁定插件（MIT，ActorComponent 挂即用）：最近目标、遮挡/超距断锁、切换目标。做锁定功能时参考或集成。
3. **Udemy：UE5 GAS Top Down RPG（Stephen Ulibarri, "Aura"课）** — https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/
   最系统 GAS 视频课（70+h，C++ 打底 + 蓝图扩展）。跟做仓库结构参考：https://github.com/hayoonleeMe/UE5_Gameplay_Ability_System_Aura 。第 2–6 周选看。
4. **i-frames 专题** — https://www.quodsoler.com/blog/i-frames-are-animation-data-not-code
   共识做法：翻滚 GA 播蒙太奇 → Anim Notify State 窗口内 Apply 带 `State.Invulnerable` Tag 的 GE → 伤害管线检查该 Tag 免伤。"无敌窗是动画数据不是代码"。同作者 GAS 资源清单：https://www.quodsoler.com/blog/unreal-gameplay-abilities-system-list-of-resources

补充：Pyrrhulla/UE5_Melee_Soulslike-（纯蓝图魂类战斗，玩法拆解参考）https://github.com/Pyrrhulla/UE5_Melee_Soulslike-

## 5. 纯蓝图 vs 必须 C++ 的边界（UE5.8 现状）

**必须 C++：** AttributeSet/属性定义（官方不支持蓝图创建）；ASC 初始化胶水；自定义 ExecutionCalculation/MMC；自定义 AbilityTask。

**可以纯蓝图：** GameplayAbility 逻辑（PlayMontage + WaitGameplayEvent 节点链）、GameplayEffect 数值配置、GameplayCue、GameplayTag 管理、UI 读属性。即 C++ 骨架写一次，之后 90% 策划向工作在蓝图。

第三方绕行（不建议用于求职 demo）：GAS Companion（Fab 付费）https://www.fab.com/listings/72e2bc50-658e-43cd-bd40-535f46d2f113 ；GAS Associate（免费）https://github.com/archangel4031/GASAssociate 。

## 6. 社区共识：从零搭建 GAS 魂类 demo 十步

1. C++ 第三人称工程，启用 GameplayAbilities 插件，Build.cs 加三模块。
2. 最小 C++ 骨架：Character 挂 ASC + IAbilitySystemInterface；AttributeSet（Health/Stamina）+ ATTRIBUTE_ACCESSORS + PostGameplayEffectExecute。
3. 规划 GameplayTag 层级：`Ability.Attack.Light`、`Ability.Dodge`、`State.Invulnerable`、`State.Stunned`、`Event.Montage.*`。
4. Enhanced Input → TryActivateAbility（demo 级直接绑定即可）。
5. 第一个 GE：初始化属性 + 伤害 GE（先 Modifier，后升级 ExecutionCalculation）。
6. 第一个攻击 GA（蓝图）：PlayMontage + WaitGameplayEvent（AnimNotify 发 Event Tag）→ trace 命中 → Apply 伤害 GE。
7. 体力：Cost GE 扣 Stamina，Duration/Periodic GE 回复。
8. 翻滚 + 无敌帧：AnimNotifyState 窗口内 Apply `State.Invulnerable` GE，伤害管线查 Tag。
9. 敌人：挂 ASC，受击硬直/削韧用 Tag + GE 表达。
10. 锁定 + UI：锁定打分/TargetSystemPlugin；UMG 绑属性委托。之后可选：篝火重生、异常状态、格挡弹反。

## 注意事项

- tranek 示例停 UE5.3，需自行升级；Narxim 已确认 5.6；UE5.8 上个别弃用 API 可能需小改。
- 第三方插件对 5.7/5.8 适配用前核对更新日期。

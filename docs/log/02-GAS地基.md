# 行动日志 02 · GAS 地基

> 状态:✅ 完成(2026-10-07)
> 产出:MyAttributeSet + BaseCharacter(C++)、GameplayTag 三根、GE_InitAttributes、BP_BaseCharacter
> 深度讲解:开发者笔记 01(概念+代码)、笔记 02(逐行精读)

## 目标

GAS 最小骨架:属性表(HP/体力)+ 战斗大脑(ASC)+ 初始化 GE + 标签体系,PIE 可见数值。

## 行动步骤

### 1. C++ 两个类(AI 编写)

- `UMyAttributeSet`:Health/MaxHealth/Stamina/MaxStamina + 元属性 Damage;`ATTRIBUTE_ACCESSORS` 宏;`PreAttributeChange` 钳制;`PostGameplayEffectExecute` 拆伤害包裹(读 Damage → 清零 → 扣血)
- `ABaseCharacter`:挂 ASC + 属性表,实现 IAbilitySystemInterface,BeginPlay 三步(建上下文 → 生成 Spec → 施加)打初始化 GE
- 编译(Ctrl+Alt+F11 热编译或关编辑器 UBT)

### 2. GameplayTag 根(直接写 Config,不进编辑器)

`Config/DefaultGameplayTags.ini` 写 Ability / State / Event 三根(子标签后续章节追加)。

### 3. 编辑器资产(用户操作)

- **GE_InitAttributes**:GameplayEffect,Instant,4 条修饰符(MaxHealth/Health/MaxStamina/Stamina 全 Override 100)。**Damage 不加**(它是临时通道)
- **BP_BaseCharacter**:继承 BaseCharacter;Init Attributes Effect = GE_InitAttributes;Auto Possess Player = Player 0;骨骼网格 = Paragon Greystone(变换 -90/-90 对齐胶囊体)
- 放进关卡

## 踩坑

- **P11**:C2027 未定义 FGameplayEffectModCallbackData → 补 `#include "GameplayEffectExtension.h"`
- **P14**(大坑):GE 里找不到自定义属性、蓝图搜不到 C++ 类、日志零报错 → `.uproject` 的 `"Modules"` 数组被编辑器覆写为空,游戏模块静默不加载 → 补回注册重启
- showdebug 不显示 → 需在 **PIE 运行时**且**附身正确**;`AbilitySystem.Debug.NextTarget` 切目标

## 验收

PIE → 控制台 `showdebug abilitysystem` → Health/Stamina 显示 100。调试面板双通道绘制原理见笔记 02 附录。

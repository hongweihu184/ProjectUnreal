# 目录整理 + 蒙太奇复制(编辑器关闭后无头运行)
import unreal

LOG = "[Reorg]"
eal = unreal.EditorAssetLibrary

SRC_ANIM = "/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations"
COMBO_DIR = "/Game/Gameplay/Anims/Combo"

# 1. 复制三段蒙太奇到 Anims/Combo/
for n in ["Attack_PrimaryA_Montage", "Attack_PrimaryB_Montage", "Attack_PrimaryC_Montage"]:
    dst = f"{COMBO_DIR}/{n}"
    if eal.does_asset_exist(dst):
        unreal.log(f"{LOG} 已存在,跳过: {dst}")
        continue
    ok = eal.duplicate_asset(f"{SRC_ANIM}/{n}", dst)
    unreal.log(f"{LOG} 复制 {n}: {ok}")

# 2. 目录整理(走正规改名管线,自动修引用)
moves = [
    ("/Game/Gameplay/BP_BaseCharacter", "/Game/Gameplay/Characters/BP_BaseCharacter"),
    ("/Game/Gameplay/GE_InitAttributes", "/Game/Gameplay/GAS/Effects/GE_InitAttributes"),
    ("/Game/Gameplay/GE_Damage", "/Game/Gameplay/GAS/Effects/GE_Damage"),
    ("/Game/Gameplay/GE_Cost", "/Game/Gameplay/GAS/Effects/GE_Cost"),
    ("/Game/Gameplay/GE_ComboWindow", "/Game/Gameplay/GAS/Effects/GE_ComboWindow"),
    ("/Game/Gameplay/GameplayAbility/GA_Attack_Light_1", "/Game/Gameplay/GAS/Abilities/GA_Attack_Light_1"),
    ("/Game/Gameplay/ABP_Greystone", "/Game/Gameplay/Anims/ABP_Greystone"),
    ("/Game/Gameplay/BS_Jog", "/Game/Gameplay/Anims/BS_Jog"),
    ("/Game/Gameplay/AN_SendGameplayEvent", "/Game/Gameplay/Anims/AN_SendGameplayEvent"),
]
for src, dst in moves:
    if not eal.does_asset_exist(src):
        unreal.log(f"{LOG} 源不存在,跳过: {src}")
        continue
    if eal.does_asset_exist(dst):
        unreal.log(f"{LOG} 目标已存在,跳过: {dst}")
        continue
    ok = eal.rename_asset(src, dst)
    unreal.log(f"{LOG} 移动 {src} -> {dst}: {ok}")

# 3. 汇总
for p in [f"{COMBO_DIR}/Attack_PrimaryA_Montage",
          "/Game/Gameplay/Characters/BP_BaseCharacter",
          "/Game/Gameplay/GAS/Effects/GE_Damage",
          "/Game/Gameplay/GAS/Abilities/GA_Attack_Light_1",
          "/Game/Gameplay/Anims/ABP_Greystone"]:
    unreal.log(f"{LOG} 存在性 {p}: {eal.does_asset_exist(p)}")
unreal.log(f"{LOG} 完成")

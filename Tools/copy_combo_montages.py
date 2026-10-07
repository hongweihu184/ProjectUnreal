import unreal

LOG = "[Combo]"
eal = unreal.EditorAssetLibrary

SRC = "/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations"
DST = "/Game/Gameplay/Anims/Combo"

for n in ["Attack_PrimaryA_Montage", "Attack_PrimaryB_Montage", "Attack_PrimaryC_Montage"]:
    dst = f"{DST}/{n}"
    obj = eal.duplicate_asset(f"{SRC}/{n}", dst)
    unreal.log(f"{LOG} 复制 {n}: {'成功' if obj else '失败'}")
    saved = eal.save_asset(dst)
    unreal.log(f"{LOG} 保存 {n}: {saved}")

import os
for n in ["Attack_PrimaryA_Montage", "Attack_PrimaryB_Montage", "Attack_PrimaryC_Montage"]:
    p = f"E:/UE5.8Project/ProjectUnreal/Content/Gameplay/Anims/Combo/{n}.uasset"
    unreal.log(f"{LOG} 磁盘存在 {n}: {os.path.exists(p)}")

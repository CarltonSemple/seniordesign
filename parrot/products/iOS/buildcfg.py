#===============================================================================
# iOS tasks.
#===============================================================================

import dragon
import os

def build_app(task=None, args=None):
    arfreeflight_ios_dir = os.path.join(dragon.WORKSPACE_DIR, "packages",
            "ARFreeFlight", "iOS", "ARFreeFlight")

    # Build application
    cmd = "xcodebuild "
    cmd += "-project ARFreeFlight.xcodeproj "
    # TODO: archive does not work well with deps
    # TODO: -derivedDataPath works but result is only partial so not reammy useful
#    cmd += "-scheme ARFreeFlight "
#    cmd += "-derivedDataPath %s " % os.path.join(dragon.OUT_DIR, "ARFreeFlight")
#    cmd += "archive -archivePath %s " % os.path.join(dragon.OUT_DIR, "ARFreeFlight.xcarchive")
    if args:
        cmd += " ".join(args)
    dragon.exec_dir_cmd(dirpath=arfreeflight_ios_dir, cmd=cmd)

    #  TODO: Export ipa file (require archive)
#    cmd = "xcodebuild "
#    cmd += "-exportArchive -exportFormat ipa "
#    cmd += "-archivePath build/ARFreeFlight.xcarchive "
#    cmd += "-exportPath build/ARFreeFlight.ipa "
#    cmd += "-exportProvisioningProfile 'iOS Team Provisioning Profile: *' "
#    dragon.exec_dir_cmd(dirpath=arfreeflight_ios_dir, cmd=cmd)

#===============================================================================
# Tasks
#===============================================================================

dragon.add_meta_task(
    name = "build-sdk",
    desc = "Build SDK",
    subtasks = ["build"],
)

dragon.add_meta_task(
    name = "build-app",
    desc = "Build SDK & generate application",
    subtasks = ["build-sdk"],
    posthook = build_app,
)

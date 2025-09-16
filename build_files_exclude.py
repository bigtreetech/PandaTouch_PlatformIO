Import("env")

must_exist=False

# Get custom_ list of files to skip
#https://docs.platformio.org/en/latest/scripting/middlewares.html

custom_build_files_exclude = env.GetProjectOption("custom_build_files_exclude")
print(" ** Custom_ skip build targets** ", custom_build_files_exclude )

def skip_tgt_from_build(env, node):
# to ignore file from a build process, just return None
    print("Skipping")
    return None

# iterate over all files
temp = custom_build_files_exclude.split(" ")
for value in temp:
    env.AddBuildMiddleware(skip_tgt_from_build, value)
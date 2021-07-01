#include <napi.h>
#include <sys/statfs.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <string.h>
#include <linux/magic.h>
#include <sys/vfs.h>

static const struct
{
    unsigned long magic;
    const char *type;
} types[] = {
    {0xadf5, "adfs"}, {0xadff, "affs"}, {0x5346414f, "afs"}, {0x09041934, "anon"}, {0x0187, "autofs"}, {0x62646576, "bdevfs"}, {0x42465331, "befs"}, {0x1badface, "bfs"}, {0x42494e4d, "binfmtfs"}, {0xcafe4a11, "bpf"}, {0x9123683e, "btrfs"}, {0x73727279, "btrfs"}, {0x27e0eb, "cgroup"}, {0x63677270, "cgroup2"}, {0xff534d42, "cifs"}, {0x73757245, "coda"}, {0x012ff7b7, "coh"}, {0x28cd3d45, "cramfs"}, {0x64626720, "debugfs"}, {0x1373, "devfs"}, {0x1cd1, "devpts"}, {0xf15f, "ecryptfs"}, {0xde5e81e4, "efivarfs"}, {0x00414a53, "efs"}, {0x137d, "ext"}, {0xef51, "ext2"}, {0xef53, "ext2"}, {0xef53, "ext3"}, {0xef53, "ext4"}, {0xf2f52010, "f2fs"}, {0x65735546, "fuse"}, {0xbad1dea, "futexfs"}, {0x4244, "hfs"}, {0x00c0ffee, "hostfs"}, {0xf995e849, "hpfs"}, {0x958458f6, "hugetlbfs"}, {0x9660, "isofs"}, {0x72b6, "jffs2"}, {0x3153464a, "jfs"}, {0x137f, "minix"}, {0x138f, "minix"}, {0x2468, "minix2"}, {0x2478, "minix2"}, {0x4d5a, "minix3"}, {0x19800202, "mqueue"}, {0x4d44, "msdos"}, {0x11307854, "mtd"}, {0x564c, "ncp"}, {0x6969, "nfs"}, {0x3434, "nilfs"}, {0x6e736673, "nsfs"}, {0x5346544e, "ntfs"}, {0x7461636f, "ocfs2"}, {0x9fa1, "openprom"}, {0x794c7630, "overlayfs"}, {0x50495045, "pipefs"}, {0x9fa0, "proc"}, {0x6165676c, "pstorefs"}, {0x002f, "qnx4"}, {0x68191122, "qnx6"}, {0x858458f6, "ramfs"}, {0x52654973, "reiserfs"}, {0x7275, "romfs"}, {0x73636673, "securityfs"}, {0xf97cff8c, "selinux"}, {0x43415d53, "smack"}, {0x517b, "smb"}, {0xfe534d42, "smb2"}, {0x534f434b, "sockfs"}, {0x73717368, "squashfs"}, {0x62656572, "sysfs"}, {0x012ff7b6, "sysv2"}, {0x012ff7b5, "sysv4"}, {0x01021994, "tmpfs"}, {0x74726163, "tracefs"}, {0x15013346, "udf"}, {0x00011954, "ufs"}, {0x9fa2, "usbdevice"}, {0x01021997, "v9fs"}, {0xa501fcf5, "vxfs"}, {0xabba1974, "xenfs"}, {0x012ff7b4, "xenix"}, {0x58465342, "xfs"}, {0x012fd16d, "_xiafs"}};

const char *get_type(unsigned long magic)
{
    static const char *unkown = "unkown";
    unsigned int i;

    for (i = 0; i < sizeof(types) / sizeof(types[0]); i++)
        if (types[i].magic == magic)
            return types[i].type;

    return unkown;
}

Napi::Value fsType(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments(only one argument)")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Wrong arguments(only string and it is a file path. e.g: C:\\abcd)").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string filePath = info[0].As<Napi::String>().ToString();

    struct statfs fileStat;
    if (statfs(filePath.data(), &fileStat) == 0)
    {
        return Napi::String::New(env, get_type((unsigned long)fileStat.f_type));
    }
    else
    {
        return Napi::String::New(env, "unknow");
    }

    return Napi::String::New(env, "unkown");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "fsType"), Napi::Function::New(env, fsType));
    return exports;
}

NODE_API_MODULE(addon, Init)

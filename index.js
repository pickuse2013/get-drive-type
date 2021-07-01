var addon = require('bindings')('addon.node')
var opsys = process.platform;

module.exports = (path) => {
    // check is system is osx or linux
    if (opsys == "darwin" || opsys == "linux") {
        // do nothing
    }

    // check is system is windows
    if (opsys == "win32" || opsys == "win64") {
        if (path.startsWith("/")) {
            return "unknow";
        }

        // windows api only take like c:\
        if (RegExp('^([a-zA-Z]):').test(path)) {
            // take c:\ only
            path = path.slice(0, 3)
        }
    }

    // on windows they are only \\192.168.0.1 or Z:\ (mount) is network drive
    // test url is start with \\??????
    if (RegExp('^\\\\').test(path)) {
        return "smb";
    }

    let result = addon.fsType(path);

    if (result) {
        return result.toString().toLowerCase();
    }

    return "unknow";
}
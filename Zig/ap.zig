const std = @import("std");

// const ArgParser = fn (args: std.process.Environ.Map) anyerror!type;
const ArgParser = fn () anyerror!void;

pub fn parse_args(comptime arg_type: type) ArgParser {
    // @compileLog(arg_type);
    return struct {
        // pub fn func(args: std.process.Environ.Map) anyerror!type {
        pub fn func() anyerror!void {
            std.debug.print("\n\nNAME: \"{}\"\n\n", .{@typeInfo(@TypeOf(arg_type))});
            // return arg_type;
        }
    }.func;
}

// pub fn parse_args(comptime arg_type: type, args: std.process.Environ.Map) !type {
//     std.debug.print("\n\n{}\n\n", .{ @typeName(arg_type) });
//     _ = args;
//     return arg_type;
// }

const x = struct {
    x: u8,
};

pub fn main(init: std.process.Init) !void {

    // const h: std.process.Init = undefined;
    // h.environ_map

    _ = init;
    const t = parse_args(x);
    try t();
    // _ = t;
}

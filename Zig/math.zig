const std = @import("std");

pub fn sum(comptime Type: type, a: Type, b: Type) Type {
    return a + b;
}

test "sum" {
    std.debug.print("\n", .{});
    defer std.debug.print("\n", .{});

    std.debug.print("\nsum_comptime_int: {} + {} = {}\n", .{ 5, 6, sum(comptime_int, 5, 6) });
    std.debug.print("\nsum_u8: {} + {} = {}\n", .{ 5, 6, sum(u8, 5, 6) });
    std.debug.print("\nsum_usize: {} + {} = {}\n", .{ 5, 6, sum(usize, 5, 6) });
    std.debug.print("\nsum_f32: {} + {} = {}\n", .{ 5.1, 6.1, sum(f32, 5.1, 6.1) });
    std.debug.print("\nsum_f64: {} + {} = {}\n", .{ 5.1, 6.1, sum(f64, 5.1, 6.1) });
}

pub fn sin(x: f64) f64 {
    const x_bound = @mod(x, 2*std.math.pi);
    const terms = 12;

    var ret: f64 = x_bound;
    var sign = false;
    var fac: f64 = 1.0;
    var exp = x_bound;

    for(2..terms*2) |i| {
        fac *= @floatFromInt(i);
        exp *= x_bound;
        if(@rem(i, 2) != 0) {
            ret = ret + (if(sign) @as(f64, 1.0) else @as(f64, -1.0))*exp/fac;
            sign = !sign;
        }
    }

    return ret;
}

pub fn cos(x: f64) f64 {
    const s = sin(x);
    return 1.0 - s*s;
}

const value: f64 = std.math.pi;

test "sin" {
    std.debug.print("\n", .{});
    defer std.debug.print("\n", .{});

    const s = sin(value);
    const s_std = std.math.sin(@as(f64, value));

    std.debug.print("\ns: {}\ns_std: {}\ndiff: {}\n", .{ s, s_std, s_std - s });
}

test "cos" {
    std.debug.print("\n", .{});
    defer std.debug.print("\n", .{});

    const c = cos(value);
    const c_std = std.math.cos(@as(f64, value));

    std.debug.print("\nc: {}\nc_std: {}\ndiff: {}\n", .{ c, c_std, c_std - c });
}


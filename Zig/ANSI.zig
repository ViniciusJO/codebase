// Runtime-agnostic ANSI escape codes for console styling

// "\x1b["
const std = @import("std");

// ─────────────────────────── Reset ───────────────────────────

pub const reset = "\x1b[0m";

// ─────────────────────────── Text styles ───────────────────────────

pub const style = struct {
    pub const bold = "\x1b[1m";
    pub const dim = "\x1b[2m";
    pub const italic = "\x1b[3m";
    pub const underline = "\x1b[4m";
    pub const blink = "\x1b[5m";
    pub const inverse = "\x1b[7m";
    pub const hidden = "\x1b[8m";
    pub const strikethrough = "\x1b[9m";
    pub const boldoff = "\x1b[22m";
    pub const dimoff = "\x1b[22m";
    pub const italicoff = "\x1b[23m";
    pub const underlineoff = "\x1b[24m";
    pub const blinkoff = "\x1b[25m";
    pub const inverseoff = "\x1b[27m";
    pub const hiddenoff = "\x1b[28m";
    pub const strikethroughoff = "\x1b[29m";
};

// ─────────────────────────── Foreground colors ───────────────────────────

pub const fg = struct {
    pub const black = "\x1b[30m";
    pub const red = "\x1b[31m";
    pub const green = "\x1b[32m";
    pub const yellow = "\x1b[33m";
    pub const blue = "\x1b[34m";
    pub const magenta = "\x1b[35m";
    pub const cyan = "\x1b[36m";
    pub const white = "\x1b[37m";

    pub const brightblack = "\x1b[90m";
    pub const brightred = "\x1b[91m";
    pub const brightgreen = "\x1b[92m";
    pub const brightyellow = "\x1b[93m";
    pub const brightblue = "\x1b[94m";
    pub const brightmagenta = "\x1b[95m";
    pub const brightcyan = "\x1b[96m";
    pub const brightwhite = "\x1b[97m";

    pub const reset = "\x1b[39m";
};

// ─────────────────────────── Background colors ───────────────────────────

pub const bg = struct {
    pub const black = "\x1b[40m";
    pub const red = "\x1b[41m";
    pub const green = "\x1b[42m";
    pub const yellow = "\x1b[43m";
    pub const blue = "\x1b[44m";
    pub const magenta = "\x1b[45m";
    pub const cyan = "\x1b[46m";
    pub const white = "\x1b[47m";

    pub const brightblack = "\x1b[100m";
    pub const brightred = "\x1b[101m";
    pub const brightgreen = "\x1b[102m";
    pub const brightyellow = "\x1b[103m";
    pub const brightblue = "\x1b[104m";
    pub const brightmagenta = "\x1b[105m";
    pub const brightcyan = "\x1b[106m";
    pub const brightwhite = "\x1b[107m";

    pub const reset = "\x1b[49m";
};

// ─────────────────────────── True color (24-bit RGB) ───────────────────────────

pub fn fg_rgb(r: u8, g: u8, b: u8) ![20]u8 {
    var color: [20]u8 = undefined;
    var stream_buff = std.io.fixedBufferStream(&color);
    var stream_writer = stream_buff.writer();
    try stream_writer.print("\x1b[38;2;{};{};{}m", .{ r, g, b });
    return color;
}

pub fn bg_rgb(r: u8, g: u8, b: u8) ![20]u8 {
    var color: [20]u8 = undefined;
    var stream_buff = std.io.fixedBufferStream(&color);
    var stream_writer = stream_buff.writer();
    try stream_writer.print("\x1b[48;2;{};{};{}m", .{ r, g, b });
    return color;
}

pub fn fg_hex(color: u32) ![20]u8 {
    return fg_rgb((color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF);
}

pub fn bg_hex(color: u32) ![20]u8 {
    return bg_rgb((color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF);
}

// ─────────────────────────── 256-color support ───────────────────────────

pub fn fg_256(color_256: u8) ![12]u8 {
    var color: [12]u8 = undefined;
    var stream_buff = std.io.fixedBufferStream(&color);
    var stream_writer = stream_buff.writer();
    try stream_writer.print("\x1b[38;5;{}m", .{ color_256 });
    return color;
}

pub fn bg_256(color_256: u8) ![12]u8 {
    var color: [12]u8 = undefined;
    var stream_buff = std.io.fixedBufferStream(&color);
    var stream_writer = stream_buff.writer();
    try stream_writer.print("\x1b[48;5;{}m", .{ color_256 });
    return color;
}

// ─────────────────────────── Cursor ───────────────────────────

pub const cursor = struct {
    pub const hide = "\x1b[?25l";
    pub const show = "\x1b[?25h";
    pub const save = "\x1b7";
    pub const restore = "\x1b8";

    pub fn position(row: u32, col: u32) ![16]u8 {
        var _command: [16]u8 = undefined;
        var stream_buff = std.io.fixedBufferStream(&_command);
        var stream_writer = stream_buff.writer();
        try stream_writer.print("\x1b[{};{}H", .{ row, col });
        return _command;
    }

    pub fn command(leter: u8, n: u32) ![16]u8 {
        var _command: [16]u8 = undefined;
        var stream_buff = std.io.fixedBufferStream(&_command);
        var stream_writer = stream_buff.writer();
        try stream_writer.print("\x1b[{}{c}", .{ n, leter });
        return _command;
    }

    pub const Commands = enum {
        up, down, forward, back, nextline, prevline, column,
        const Self = @This();
        pub fn code(self: *Self) u8 {
            switch (self) {
                .up => "A",
                .down => "B",
                .forward => "C",
                .back => "D",
                .nextline => "E",
                .prevline => "F",
                .column => "G",
            }
        }
    };

    pub fn up(n: u32) ![16]u8 { return cursor.command(cursor.Commands.up.code(), n); }
    pub fn down(n: u32) ![16]u8 { return cursor.command(cursor.Commands.down.code(), n); }
    pub fn forward(n: u32) ![16]u8 { return cursor.command(cursor.Commands.forward.code(), n); }
    pub fn back(n: u32) ![16]u8 { return cursor.command(cursor.Commands.back.code(), n); }
    pub fn nextline(n: u32) ![16]u8 { return cursor.command(cursor.Commands.nextline.code(), n); }
    pub fn prevline(n: u32) ![16]u8 { return cursor.command(cursor.Commands.prevline.code(), n); }
    pub fn column(n: u32) ![16]u8 { return cursor.command(cursor.Commands.column.code(), n); }
};

// ─────────────────────────── Clear screen / line ───────────────────────────

pub const clear = struct {
		pub const screen = "\x1b[2j";
		pub const screendown = "\x1b[0j";
		pub const screenup = "\x1b[1j";
		pub const line = "\x1b[2k";
		pub const lineright = "\x1b[0k";
		pub const lineleft = "\x1b[1k";
};

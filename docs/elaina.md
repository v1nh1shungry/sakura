# Introduction

`elaina` is the script language of `sakura`. `elaina` is quite simple, and it is not even a programing language.

# Syntax

## Comment

A sentence beginning with a `;` is a line-comment.

```ela
; This is a comment
```

## Integer literal

Currently only decimal integers, no floating point numbers are supported.

```ela
; This is an integer literal
10
```

## String literal

Text surrounded by `"` is a string literal. Attention, there are no escape characters. And if you wrap line, then the string literal will also wrap.

```ela
; This is a string literal
"Hello, world!"

; This is same as "Hello, \nworld!" in C
"Hello, 
world!"
```

## Identifier

An identifier starts with a `$`. It is used to name a variable.

```
; This is an identifier
$a
```

## Arithmetic

`+`, `-`, `*`, `/` are supported. Note that thera are only integers.

```ela
1 + 2 * 3
```

## Comparative Operations

`<`, `>`, `<=`, `>=`, `==`, `!=` are supported. Note that they all return an integer. `1` means `true` and `0` means `false`, like C.

```ela
1 < 2
```

## Assignment

Since we have variables, we can assign a value to a variable. Note that a variable can only be assigned to an integer value.

```ela
$a := 10
```

## Command

A command starts with a `@`. They are all provided by the system. You can't build a command yourself. The arguments can be integer literal or string literal.

```ela
@jump "start.ela"
```

Note that a `command` is not a function. It will not return any value. So you cannot do this:

```ela
; If you do so, it is equal to
; @foo
; @bar "foobar"
@foo @bar "foobar"
```

Since assignment will not return any values too, assignment actually is kind of command. Just syntax sugar.

## Name block

Let's make it clear. It is just a syntax sugar and is actually a command. It allows us to write this:

```
[vinh]
"I love Elaina!"
```

It will be printed on a `textbox`. Note that the `name` can be empty. And make sure a string literal is followed by the name block.

## Summary

You should keep in mind that you can only write three types of statement (actually one),

* command call
* assignment
* name block syntax

You cannot just write

```ela
1 + 2
```

which does nothing at all, and it is even seen as an error!

# Commands

Here are all commands provided by the system.

* **@bacground [image file path]**: set the specified image as background.
* **@bgm [bgm file path]**: set the specified music as bgm.
* **@pauseBgm [bgm file path]**: pause the specified bgm.
* **@scene [scene config file path]**
* **@addSprite [name] [texture file path] [left] [top]**: add a sprite. You should name the sprite so that you can remove it latter.
* **@rmSprite [name]**
* **@select [first text] [first action] [second text] [second action]**: set up the selectors. Note that `action` should be a script file path.
* **@if [condition] [conseq] [alt]**: `condition` should be an integer (0 is false, 1 is true). `conseq` and `alt` should be script file path.
* **@jump [script file path]**: jump to the beginning of the specified script.

## Example

```ela
; So first you should load a scene to tell the information of GUI
@load "start.ela"

; Play background music, which will loop
@bgm "bgm.ogg"

; Someone says something
; This will block the script engine until you press the mouse button
[vinh]
"Hi, Elaina!"

; add a sprite
@addSprite "elaina" "elaina.png" 10 10

[Elaina]
"Hi, vinh!"

; remove a sprite, so that we can draw another sprite
@rmSprite "elaina"

; launch a selection
; This will block the script engine
@select "Yes" "a.ela" "No" "b.ela"

; Anything below @select will never be executed for we will jump to another script
```

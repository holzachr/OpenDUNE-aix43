/* $Id$ */

#include <assert.h>
#include <stdio.h>
#include "types.h"
#include "libemu.h"
#include "../global.h"
#include "../pool/unit.h"
#include "../structure.h"
#include "../unit.h"
#include "widget.h"

extern void f__22A6_034F_000C_5E0A();
extern void f__24D0_000D_0039_C17D();
extern void f__2598_0000_0017_EB80();
extern void f__260F_003A_0014_CA10();
extern void f__2B6C_0197_00CE_4D32();
extern void f__2B6C_0292_0028_3AD7();
extern void emu_GUI_DrawText_Wrapper();
extern void emu_GUI_DrawWiredRectangle();
extern void emu_GUI_DrawSprite();
extern void emu_GUI_GetShortcut();
extern void emu_GUI_String_Get_ByIndex();
extern void emu_GUI_Update97E5();
extern void emu_GUI_Widget_DrawBorder();
extern void emu_GUI_Widget_DrawBorder2();
extern void emu_String_Get_ByIndex();
extern void overlay(uint16 cs, uint8 force);

/**
 * Draw a text button widget to the display, relative to its parent.
 *
 * @param w The widget (which is a button) to draw.
 */
void GUI_Widget_TextButton_Draw(Widget *w)
{
	uint16 old6C91;
	uint16 positionX, positionY;
	uint16 width, height;
	uint16 state, colour;

	if (w == NULL) return;

	emu_push(2);
	emu_push(emu_cs); emu_push(0x0F79); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	/* Check if this overlay should be reloaded */
	if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
	emu_sp += 2;
	old6C91 = emu_ax;

	positionX = w->offsetX + (g_global->variable_4062[w->parentID][0] << 3);
	positionY = w->offsetY +  g_global->variable_4062[w->parentID][1];
	width     = w->width;
	height    = w->height;

	g_global->variable_4062[19][0] = positionX >> 3;
	g_global->variable_4062[19][1] = positionY;
	g_global->variable_4062[19][2] = width >> 3;
	g_global->variable_4062[19][3] = height;

	state  = ((w->state & 0x0001) != 0) ? 0 : 2;
	colour = ((w->state & 0x0004) != 0) ? 0xE7 : 0xE8;

	emu_push(1);
	emu_push(state);
	emu_push(19);
	emu_push(emu_cs); emu_push(0x102E); emu_cs = 0x10E4; emu_GUI_Widget_DrawBorder();
	/* Check if this overlay should be reloaded */
	if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
	emu_sp += 6;

	if (w->stringID == 0x1E || w->stringID == 0x66 || w->stringID == 0x6B || w->stringID == 0x6C) {
		emu_push(0x122);
		emu_push(0x0);
		emu_push(colour);
		emu_push(positionY + 2);
		emu_push(positionX + (width / 2));
	} else {
		emu_push(0x22);
		emu_push(0x0);
		emu_push(colour);
		emu_push(positionY + 2);
		emu_push(positionX + 3);
	}

	emu_push(w->stringID);
	emu_push(emu_cs); emu_push(0x1097); overlay(0x34F2, 0); emu_GUI_String_Get_ByIndex();
	emu_sp += 2;

	emu_push(emu_dx); emu_push(emu_ax);
	emu_push(emu_cs); emu_push(0x109F); emu_cs = 0x10E4; emu_GUI_DrawText_Wrapper();
	/* Check if this overlay should be reloaded */
	if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
	emu_sp += 14;

	if (old6C91 == 0) {
		emu_push(positionY + height);
		emu_push(positionX + width);
		emu_push(positionY);
		emu_push(positionX);
		emu_push(emu_cs); emu_push(0x10BB); emu_cs = 0x2B6C; f__2B6C_0197_00CE_4D32();
		/* Check if this overlay should be reloaded */
		if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
		emu_sp += 8;

		emu_push(0);
		emu_push(2);
		emu_push(height);
		emu_push(width >> 3);
		emu_push(positionY);
		emu_push(positionX >> 3);
		emu_push(positionY);
		emu_push(positionX >> 3);
		emu_push(emu_cs); emu_push(0x10E5); emu_cs = 0x24D0; f__24D0_000D_0039_C17D();
		/* Check if this overlay should be reloaded */
		if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
		emu_sp += 16;

		emu_push(emu_cs); emu_push(0x10ED); emu_cs = 0x2B6C; f__2B6C_0292_0028_3AD7();
		/* Check if this overlay should be reloaded */
		if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
	}

	emu_push(old6C91);
	emu_push(emu_cs); emu_push(0x10F5); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	/* Check if this overlay should be reloaded */
	if (emu_cs == 0x34F2) { overlay(0x34F2, 1); }
	emu_sp += 2;

	emu_ax = 0;
}

/**
 * Draw a sprite button widget to the display, relative to 0,0.
 *
 * @param w The widget (which is a button) to draw.
 */
void GUI_Widget_SpriteButton_Draw(Widget *w)
{
	uint16 old6C91;
	uint16 positionX, positionY;
	uint16 width, height;
	uint16 spriteID;
	bool buttonDown;

	assert(g_global->variable_6624.csip == 0x22A6034F);

	if (w == NULL) return;

	spriteID = 0;
	if (g_global->selectionUnit.csip != 0x0) {
		UnitInfo *ui;
		Unit *u;

		u = Unit_Get_ByMemory(g_global->selectionUnit);
		ui = &g_unitInfo[u->type];

		spriteID = ui->spriteID;
	} else {
		StructureInfo *si;
		Structure *s;

		s = Structure_Get_ByPackedTile(g_global->selectionPosition);
		if (s == NULL) return;
		si = &g_structureInfo[s->type];

		spriteID = si->spriteID;
	}

	old6C91 = g_global->variable_6C91;
	if (old6C91 == 0) {
		emu_push(2);
		emu_push(emu_cs); emu_push(0x0CC9); emu_cs = 0x2598; f__2598_0000_0017_EB80();
		emu_sp += 2;
	}

	buttonDown = ((w->state & 0x0004) != 0) ? true : false;

	positionX = w->offsetX;
	positionY = w->offsetY;
	width     = w->width;
	height    = w->height;

	emu_push(0xC);
	emu_push(positionY + height);
	emu_push(positionX + width);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0D7F); emu_cs = 0x251B; emu_GUI_DrawWiredRectangle();
	emu_sp += 10;

	emu_push(buttonDown ? 1 : 0);
	emu_push(g_global->variable_3C3A.s.cs); emu_push(g_global->variable_3C3A.s.ip);
	emu_push(0x100);
	emu_push(0);
	emu_push(positionY);
	emu_push(positionX);
	/* TODO -- Change this into an accessable array */
	emu_push(emu_get_memory16(0x2DCE, spriteID * 4, 0x442));
	emu_push(emu_get_memory16(0x2DCE, spriteID * 4, 0x440));
	emu_push(g_global->variable_6C91);
	emu_push(emu_cs); emu_push(0x0DB5); emu_cs = 0x2903; emu_GUI_DrawSprite();
	emu_sp += 20;

	emu_push(0);
	emu_push(buttonDown ? 0 : 1);
	emu_push(height);
	emu_push(width);
	emu_push(positionY);
	emu_push(positionX);
	emu_push(emu_cs); emu_push(0x0DD1); emu_cs = 0x10E4; emu_GUI_Widget_DrawBorder2();
	emu_sp += 12;

	if (old6C91 != 0) return;

	emu_push(positionY + height + 1);
	emu_push(positionX + width + 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0DF5); emu_cs = 0x2B6C; f__2B6C_0197_00CE_4D32();
	emu_sp += 8;

	emu_push(0);
	emu_push(2);
	emu_push(height + 2);
	emu_push(width + 2);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0E27); emu_cs = 0x22A6; f__22A6_034F_000C_5E0A();
	emu_sp += 16;

	emu_push(emu_cs); emu_push(0x0E2F); emu_cs = 0x2B6C; f__2B6C_0292_0028_3AD7();

	emu_push(0);
	emu_push(emu_cs); emu_push(0x0E37); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;
}

/**
 * Draw a sprite/text button widget to the display, relative to 0,0.
 *
 * @param w The widget (which is a button) to draw.
 */
void GUI_Widget_SpriteTextButton_Draw(Widget *w)
{
	uint16 old6C91;
	Structure *s;
	uint16 positionX, positionY;
	uint16 width, height;
	uint16 spriteID;
	uint16 percentDone;
	bool buttonDown;

	assert(g_global->variable_6624.csip == 0x22A6034F);

	if (w == NULL) return;

	spriteID    = 0;
	percentDone = 0;

	s = Structure_Get_ByPackedTile(g_global->selectionPosition);
	if (s == NULL) return;

	emu_push(emu_cs); emu_push(0x0846); emu_cs = 0x10E4; emu_GUI_Update97E5();

	old6C91 = g_global->variable_6C91;
	if (old6C91 == 0) {
		emu_push(2);
		emu_push(emu_cs); emu_push(0x085B); emu_cs = 0x2598; f__2598_0000_0017_EB80();
		emu_sp += 2;
	}

	buttonDown = ((w->state & 0x0004) != 0) ? true : false;

	positionX = w->offsetX;
	positionY = w->offsetY;
	width     = w->width;
	height    = w->height;

	emu_push(0xC);
	emu_push(positionY + height);
	emu_push(positionX + width);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x08B0); emu_cs = 0x251B; emu_GUI_DrawWiredRectangle();
	emu_sp += 10;

	emu_push(1);
	emu_push(buttonDown ? 0 : 1);
	emu_push(height);
	emu_push(width);
	emu_push(positionY);
	emu_push(positionX);
	emu_push(emu_cs); emu_push(0x08CD); emu_cs = 0x10E4; emu_GUI_Widget_DrawBorder2();
	emu_sp += 12;

	switch (g_global->variable_97E5) {
		case 0x2A: /* "Launch" */
			spriteID = 0x1E;
			break;

		case 0x2B: /* "Fremen" */
			spriteID = 0x5E;
			break;

		case 0x2C: /* "Saboteur" */
			spriteID = 0x60;
			break;

		case 0x90: /* "Upgrading|%d%% done" */
		default:
			spriteID = 0x0;
			break;

		case 0x26: /* "Place it" */
		case 0x27: /* "Completed" */
		case 0x28: /* "On hold" */
		case 0x29: /* "Build it" */
		case 0x2E: /* "%d%% done" */
			if (s->type == STRUCTURE_CONSTRUCTION_YARD) {
				StructureInfo *si;
				uint16 spriteWidth;
				uint16 x, y;

				emu_push(buttonDown ? 2 : 0);
				emu_push(g_global->variable_3C3A.s.cs); emu_push(g_global->variable_3C3A.s.ip);
				emu_push(0x100);
				emu_push(0);
				emu_push(positionY + 5);
				emu_push(positionX + 37);
				emu_push(emu_get_memory16(0x2DCE, 63 * 4, 0x442));
				emu_push(emu_get_memory16(0x2DCE, 63 * 4, 0x440));
				emu_push(g_global->variable_6C91);
				emu_push(emu_cs); emu_push(0x0936); emu_cs = 0x2903; emu_GUI_DrawSprite();
				emu_sp += 20;

				emu_push(emu_get_memory16(0x2DCE, 24 * 4, 0x442));
				emu_push(emu_get_memory16(0x2DCE, 24 * 4, 0x440));
				emu_push(emu_cs); emu_push(0x0958); emu_cs = 0x260F; f__260F_003A_0014_CA10();
				emu_sp += 4;
				spriteWidth = emu_ax + 1;

				si = &g_structureInfo[s->buildingType];

				for (y = 0; y < g_global->layoutSize[si->layout][1]; y++) {
					for (x = 0; x < g_global->layoutSize[si->layout][0]; x++) {
						emu_push(0);
						emu_push(0);
						emu_push(positionY + y * spriteWidth + 6);
						emu_push(positionX + x * spriteWidth + 38);
						emu_push(emu_get_memory16(0x2DCE, 24 * 4, 0x442));
						emu_push(emu_get_memory16(0x2DCE, 24 * 4, 0x440));
						emu_push(g_global->variable_6C91);
						emu_push(emu_cs); emu_push(0x09D0); emu_cs = 0x2903; emu_GUI_DrawSprite();
						emu_sp += 14;
					}
				}

				spriteID = si->spriteID;
			} else {
				UnitInfo *ui;

				ui = &g_unitInfo[s->buildingType];
				spriteID = ui->spriteID;
			}
			break;
	}

	if (spriteID != 0) {
		emu_push(buttonDown ? 1 : 0);
		emu_push(g_global->variable_3C3A.s.cs); emu_push(g_global->variable_3C3A.s.ip);
		emu_push(0x100);
		emu_push(0);
		emu_push(positionY + 2);
		emu_push(positionX + 2);
		emu_push(emu_get_memory16(0x2DCE, spriteID * 4, 0x442));
		emu_push(emu_get_memory16(0x2DCE, spriteID * 4, 0x440));
		emu_push(g_global->variable_6C91);
		emu_push(emu_cs); emu_push(0x0A7E); emu_cs = 0x2903; emu_GUI_DrawSprite();
		emu_sp += 20;
	}

	if (g_global->variable_97E5 == 0x2E) { /* "%d%% done" */
		uint16 buildTime;
		uint16 timeLeft;

		if (s->type == STRUCTURE_CONSTRUCTION_YARD) {
			StructureInfo *si;

			si = &g_structureInfo[s->buildingType];
			buildTime = si->buildTime;
		} else if (s->type == STRUCTURE_REPAIR) {
			UnitInfo *ui;

			if (s->linkedID == 0xFF) return;

			ui = &g_unitInfo[Unit_Get_ByIndex(s->linkedID)->type];
			buildTime = ui->buildTime;
		} else {
			UnitInfo *ui;

			ui = &g_unitInfo[s->buildingType];
			buildTime = ui->buildTime;
		}

		timeLeft = buildTime - (s->countDown + 255) / 256;
		percentDone = 100 * timeLeft / buildTime;
	}

	if (g_global->variable_97E5 == 0x90) { /* "Upgrading|%d%% done" */
		percentDone = 100 - s->upgradeTimeLeft;

		emu_push(percentDone);
		emu_push(0x21);
		emu_push(0);
		emu_push(buttonDown ? 0xE : 0xF);
		emu_push(positionY + height - 19);
		emu_push(positionX + 1);

		emu_push(g_global->variable_97E5);
		emu_push(emu_cs); emu_push(0x0B69); emu_cs = 0x0FCB; emu_String_Get_ByIndex();
		emu_sp += 2;

		emu_push(emu_dx); emu_push(emu_ax);
		emu_push(emu_cs); emu_push(0x0B71); emu_cs = 0x10E4; emu_GUI_DrawText_Wrapper();
		emu_sp += 16;
	} else {
		emu_push(percentDone);
		emu_push(0x121);
		emu_push(0);
		emu_push((g_global->variable_97E5 == 0x26) ? 0xEF : (buttonDown ? 0xE : 0xF)); /* "Place it" */
		emu_push(positionY + height - 9);
		emu_push(positionX + width / 2);

		emu_push(g_global->variable_97E5);
		emu_push(emu_cs); emu_push(0x0BC1); emu_cs = 0x0FCB; emu_String_Get_ByIndex();
		emu_sp += 2;

		emu_push(emu_dx); emu_push(emu_ax);
		emu_push(emu_cs); emu_push(0x0BC9); emu_cs = 0x10E4; emu_GUI_DrawText_Wrapper();
		emu_sp += 16;
	}

	if (g_global->variable_97E5 == 0x2E || g_global->variable_97E5 == 0x90) { /* "%d%% done" / "Upgrading|%d%% done" */
		emu_push(0x28); /* "On hold" */
	} else {
		emu_push(g_global->variable_97E5);
	}
	emu_push(emu_cs); emu_push(0x0BFC); emu_cs = 0x0FCB; emu_String_Get_ByIndex();
	emu_sp += 2;

	emu_push(emu_get_memory8(emu_dx, emu_ax, 0x0));
	emu_push(emu_cs); emu_push(0x0BE4); emu_cs = 0x29DA; emu_GUI_GetShortcut();
	emu_sp += 2;
	w->shortcut = emu_ax;

	if (old6C91 != 0x0) return;

	emu_push(positionY + height + 1);
	emu_push(positionX + width + 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0C34); emu_cs = 0x2B6C; f__2B6C_0197_00CE_4D32();
	emu_sp += 8;

	emu_push(0);
	emu_push(2);
	emu_push(height + 2);
	emu_push(width + 2);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0C66); emu_cs = 0x22A6; f__22A6_034F_000C_5E0A();
	emu_sp += 16;

	emu_push(emu_cs); emu_push(0x0C6E); emu_cs = 0x2B6C; f__2B6C_0292_0028_3AD7();

	emu_push(0);
	emu_push(emu_cs); emu_push(0x0C76); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;
}

/**
 * Draw a text button widget to the display, relative to 0,0.
 *
 * @param w The widget (which is a button) to draw.
 */
void GUI_Widget_TextButton2_Draw(Widget *w)
{
	uint16 old6C91;
	uint16 stringID;
	uint16 positionX, positionY;
	uint16 width, height;
	uint16 colour;
	bool buttonSelected;
	bool buttonDown;

	assert(g_global->variable_6624.csip == 0x22A6034F);

	if (w == NULL) return;

	old6C91 = g_global->variable_6C91;
	if (old6C91 == 0) {
		emu_push(2);
		emu_push(emu_cs); emu_push(0x0E66); emu_cs = 0x2598; f__2598_0000_0017_EB80();
		emu_sp += 2;

	}

	stringID = w->stringID;

	buttonSelected = ((w->state & 0x0001) != 0) ? true : false;
	buttonDown     = ((w->state & 0x0004) != 0) ? true : false;

	positionX = w->offsetX;
	positionY = w->offsetY;
	width     = w->width;
	height    = w->height;

	emu_push(0xC);
	emu_push(positionY + height);
	emu_push(positionX + width);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0EDA); emu_cs = 0x251B; emu_GUI_DrawWiredRectangle();
	emu_sp += 10;

	emu_push(1);
	emu_push(buttonDown ? 0 : 1);
	emu_push(height);
	emu_push(width);
	emu_push(positionY);
	emu_push(positionX);
	emu_push(emu_cs); emu_push(0x0EF7); emu_cs = 0x10E4; emu_GUI_Widget_DrawBorder2();
	emu_sp += 12;

	colour = 0xF;
	if (buttonSelected) {
		colour = 0x6;
	} else if (buttonDown) {
		colour = 0xE;
	}

	if (!buttonDown && stringID == 0x22) { /* "Repair" */
		colour = 0xEF;
	}

	emu_push(0x121);
	emu_push(0);
	emu_push(colour);
	emu_push(positionY + 1);
	emu_push(positionX + width / 2);

	emu_push(stringID);
	emu_push(emu_cs); emu_push(0x0F44); emu_cs = 0x0FCB; emu_String_Get_ByIndex();
	emu_sp += 2;

	emu_push(emu_dx); emu_push(emu_ax);
	emu_push(emu_cs); emu_push(0x0F4C); emu_cs = 0x10E4; emu_GUI_DrawText_Wrapper();
	emu_sp += 14;

	emu_push(stringID);
	emu_push(emu_cs); emu_push(0x0F57); emu_cs = 0x0FCB; emu_String_Get_ByIndex();
	emu_sp += 2;

	emu_push(emu_get_memory8(emu_dx, emu_ax, 0x0));
	emu_push(emu_cs); emu_push(0x0F66); emu_cs = 0x29DA; emu_GUI_GetShortcut();
	emu_sp += 2;
	w->shortcut = emu_ax;

	if (old6C91 != 0) return;

	emu_push(positionY + height + 1);
	emu_push(positionX + width + 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0F8F); emu_cs = 0x2B6C; f__2B6C_0197_00CE_4D32();
	emu_sp += 8;

	emu_push(0);
	emu_push(2);
	emu_push(height + 2);
	emu_push(width + 2);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(positionY - 1);
	emu_push(positionX - 1);
	emu_push(emu_cs); emu_push(0x0FC1); emu_cs = 0x22A6; f__22A6_034F_000C_5E0A();
	emu_sp += 16;

	emu_push(emu_cs); emu_push(0x0FC9); emu_cs = 0x2B6C; f__2B6C_0292_0028_3AD7();

	emu_push(0);
	emu_push(emu_cs); emu_push(0x0FD1); emu_cs = 0x2598; f__2598_0000_0017_EB80();
	emu_sp += 2;
}
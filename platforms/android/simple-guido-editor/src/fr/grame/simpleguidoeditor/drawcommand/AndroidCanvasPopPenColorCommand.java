package fr.grame.simpleguidoeditor.drawcommand;

import drawcommand.PopPenColorCommand;

import android.graphics.Canvas;
import fr.grame.simpleguidoeditor.GuidoCanvasView;

public class AndroidCanvasPopPenColorCommand extends PopPenColorCommand implements DrawToCanvas {

	public AndroidCanvasPopPenColorCommand() {
		super();
	}

	@Override
	public void drawToCanvas(Canvas canvas, GuidoCanvasView view) {
		view.popPenColor();
	}
}

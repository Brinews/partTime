package chess;

import java.awt.GridLayout;

import javax.swing.JPanel;

import chess.Resources.Player;
import chess.Resources;

public class ChessBoard extends JPanel implements BoardInterface
{
	public ChessBoard(MouseListener listener)
	{
		this.setSize(400, 400);
		this.setLayout(new GridLayout(Resources.BOARD_SIZE, Resources.BOARD_SIZE));
		grids = new BoardGrid[Resources.BOARD_SIZE][Resources.BOARD_SIZE];

		for (int j = Resources.BOARD_SIZE - 1; j >= 0; j --)
		{
			for (int i = 0; i < Resources.BOARD_SIZE; i ++)
			{
				BoardLocation location = new BoardLocation(i, j);
				grids[i][j] = new BoardGrid(location);
				grids[i][j].setVisible(true);
				grids[i][j].addMouseListener(listener);
				this.add(grids[i][j]);
			}
		}
	}
	
	public ChessBoard()
	{
		this.setSize(400, 100);
		this.setLayout(new GridLayout(2, Resources.BOARD_SIZE));
		outsideGrids = new BoardGrid[Resources.NUM_PIECES_PER_PLAYER];
		for (int i = 0; i < Resources.NUM_PIECES_PER_PLAYER; i ++)
		{
			outsideGrids[i] = new BoardGrid(i);
			//outsideGrids[i].addMouseListener(listener);
			this.add(outsideGrids[i]);
		}
	}
	
	public void clearBoard()
	{
		if (grids != null)
		{
			for (int i = 0; i < Resources.BOARD_SIZE; i ++)
			{
				for (int j = 0; j < Resources.BOARD_SIZE; j ++)
				{
					grids[i][j].setEmpty();
				}
			}
		}
		if (outsideGrids != null)
		{
			for (int i = 0; i < Resources.NUM_PIECES_PER_PLAYER; i ++)
			{
				outsideGrids[i].setEmpty();
			}
		}
	}
	
	public void setLocationSelected(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setSelected();
	}
	
	public void setLocationUnSelected(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setUnSelected();
	}
	
	public void setLocationEmpty(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setEmpty();
	}
	
	public void setPiece(BoardLocation location, Piece piece)
	{
		if (location.getX() >= 0)
			grids[location.getX()][location.getY()].setPiece(piece);
		else
			outsideGrids[location.getY()].setPiece(piece);
	}
	
	public void flushLocation(BoardLocation location)
	{
		grids[location.getX()][location.getY()].repaint();
	}
	
	public boolean isWithinBoard(BoardLocation location)
	{
		return (location.getX() >= 0 && location.getX() < Resources.BOARD_SIZE && location.getY() >= 0 && location.getY() < Resources.BOARD_SIZE);
	}
	
	public boolean isEmpty(BoardLocation location)
	{
		return grids[location.getX()][location.getY()].getPiece() == null;
	}
	
	public boolean isFriendly(BoardLocation location, Player currentPlayer)
	{
		Piece piece = grids[location.getX()][location.getY()].getPiece();
		if (piece != null && piece.getPlayer() == currentPlayer)
			return true;
		return false;
	}
	
	public boolean isEnemy(BoardLocation location, Player currentPlayer)
	{
		Piece piece = grids[location.getX()][location.getY()].getPiece();
		if (piece != null && piece.getPlayer() != currentPlayer)
			return true;
		return false;
	}

	private BoardGrid [][] grids = null;
	private BoardGrid [] outsideGrids = null; 	// for captured pieces
}